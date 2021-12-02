#!/usr/bin/env python3

import argparse
import cxxfilt
import re
import sys

class Node(object):
    def __init__(self, name, stack_usage, allocation_type):
        self.__name = name
        self.__stack_usage = stack_usage
        self.__total_stack_usage = None
        self.__allocation_type = allocation_type

    @property
    def name(self):
        return self.__name

    @property
    def stack_usage(self):
        return self.__stack_usage

    @property
    def total_stack_usage(self):
        return self.__total_stack_usage

    @total_stack_usage.setter
    def total_stack_usage(self, value):
        self.__total_stack_usage = value

    @property
    def allocation_type(self):
        return self.__allocation_type

    def __eq__(self, other):
        return (self.name == other.name and self.stack_usage == other.stack_usage and self.allocation_type == other.allocation_type)
        
    def __str__(self):
        return '{} {} {}'.format(self.allocation_type, self.stack_usage, self.name)

class Edge(object):
    def __init__(self, source, target, label):
        self.__source = source
        self.__target = target
        self.__label = label

    @property
    def source(self):
        return self.__source

    @property
    def target(self):
        return self.__target

    @property
    def label(self):
        return self.__label

    def __str__(self):
        return '{} -> {}'.format(self.source, self.target)

class CallGraph(object):
    def __init__(self):
        self.__nodes = dict()
        self.__nodes['__indirect_call'] = Node('<indirect call>', None, 'unknown')
        self.__nodes['abort'] = Node('<abort>', None, 'unknown')
        self.__edges = dict()

        self.__matcher_node = re.compile('node: { title: \"(.*?)\" label: \".*?(\d*) bytes \((.*)\)\" }')
        self.__matcher_edge = re.compile('edge: { sourcename: \"(.*?)\" targetname: \"(.*?)\" label: \"(.*?)\" }')

    def try_add_node(self, line):
        match = self.__matcher_node.match(line)
        if not match:
            return False

        name = match.group(1).split(':')[-1]
        stack_usage = int(match.group(2))
        allocation_type = match.group(3)
        node = Node(name, stack_usage, allocation_type)

        if name in self.__nodes:
            other_node = self.__nodes[name]
            if node.allocation_type != other_node.allocation_type:
                raise ValueError('incompatible allocation type')

            if node.stack_usage > other_node.stack_usage:
                self.__nodes[name] = node

            return True

        self.__nodes[name] = node
        return True

    def try_add_edge(self, line):
        match = self.__matcher_edge.match(line)
        if not match:
            return False

        source = match.group(1).split(':')[-1]
        target = match.group(2).split(':')[-1]
        label = match.group(3)
        edge = Edge(source, target, label)

        if source not in self.__edges:
            self.__edges[source] = []
        self.__edges[source].append(edge)

    def get_total_stack_usage_of(self, function, ident=0, show=False):
        if function not in self.__nodes:
            if show:
                print('{} {} (0 byte)'.format(' ' * ident, function))
            return 0
        node = self.__nodes[function]

        if function not in self.__edges:
            if show:
                print('{} {} ({} bytes)'.format(' ' * ident, function, node.stack_usage))
            node.total_stack_usage = node.stack_usage
            return node.total_stack_usage

        max_child_stack_usage = 0
        for edge in self.__edges[function]:
            child_stack_usage = self.get_total_stack_usage_of(edge.target, ident + 4)
            max_child_stack_usage = max(max_child_stack_usage, child_stack_usage if child_stack_usage is not None else 0)

        if show:
            print('{}{} ({} bytes)'.format(' ' * ident, function, node.stack_usage + max_child_stack_usage))
        node.total_stack_usage = node.stack_usage + max_child_stack_usage
        return node.total_stack_usage

    def get_callees_of(self, function):
        if function not in self.__edges:
            return []
        edges = self.__edges[function]

        child_nodes = []
        for edge in edges:
            target = edge.target
            if target in self.__nodes:
                child_nodes.append(self.__nodes[target])
            else:
                child_nodes.append(None)

        return child_nodes

    def get_node_of(self, function):
        if function not in self.__nodes:
            return None

        return self.__nodes[function]

    def print_functions(self):
        for (_, node) in self.__nodes.items():
            print(node)

    def print_edges(self):
         for (_, edges) in self.__edges.items():
            for edge in edges:
                print(edge)

    def print(self):
        print_functions()
        print_edges()

if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='Script used estimating function total stack usage.'
    )
    parser.add_argument('files',
        type=str, nargs='+', metavar='FILES', default=[],
        help='the VCG files generated by -fcallgraph-info=su',
    )
    parser.add_argument('--function',
        type=str, metavar='FUNC', default='task::slaves::loop()',
        help='the function that we want to compute the stack',
    )
    parser.add_argument('--list-functions',
        action='store_true',
        help='print all functions and exit',
    )
    parser.add_argument('--non-verbose',
        action='store_true',
        help='print all functions and exit',
    )
    args = parser.parse_args()

    cg = CallGraph()
    for filename in args.files:
        with open(filename, 'r') as f:
            for line in f.readlines():
                if cg.try_add_node(line):
                    continue
                cg.try_add_edge(line)

    if args.list_functions:
        cg.print_functions()
        exit(0)

    stack_usage = cg.get_total_stack_usage_of(args.function, show=not args.non_verbose)
    if not args.non_verbose:
        print('estimated stack usage for {} is {} bytes'.format(args.function, stack_usage))
    else:
        print(stack_usage)