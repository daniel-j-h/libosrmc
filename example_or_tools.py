#!/usr/bin/env python2

from __future__ import print_function

import sys
import random

from osrmcpy import OSRM, Coordinate
from ortools.constraint_solver.pywrapcp import RoutingParameters, RoutingModel, RoutingSearchParameters


# Integration with Google's or-tools for Traveling Salesman Problems
def main():
    if len(sys.argv) != 2:
        sys.exit('Usage: {} monaco.osrm'.format(sys.argv[0]))

    osrm = OSRM(sys.argv[1])

    # 100 random coordinates (n^2 table, dummy from and to coordinate)
    n = 100
    first = 0
    last = n - 1

    # Area in Monaco dataset to sample from
    bottom_left = Coordinate(longitude=7.413194, latitude=43.731056)
    top_right = Coordinate(longitude=7.421639, latitude=43.735440)

    random_coordinate = lambda: Coordinate(longitude=random.uniform(bottom_left.longitude, top_right.longitude),
                                           latitude=random.uniform(bottom_left.latitude, top_right.latitude))

    table = osrm.table([random_coordinate() for _ in range(n)])

    if table:
        params = RoutingParameters()
        RoutingModel.SetGlobalParameters(params)

        routing = RoutingModel(n, 1, [first], [last])

        parameters = RoutingSearchParameters()
        parameters.first_solution = 'PathCheapestArc'
        parameters.no_lns = True
        parameters.no_tsp = False

        distance = lambda s, t: table[s][t]
        routing.SetArcCostEvaluatorOfAllVehicles(distance)

        solution = routing.SolveWithParameters(parameters, None)

        if solution:
            print('Solution: {0:.0f} seconds'.format(solution.ObjectiveValue()))
            # solution can be unpacked here into routes by means of:
            # routing.Start, routing.IsEnd, routing.NextVar, assignment.Value
        else:
            print('No solution found')

    else:
        print('Unable to get response from Table service')


if __name__ == '__main__':
    main()
