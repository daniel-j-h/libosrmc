#!/usr/bin/env python2

from __future__ import print_function

import sys
import random

from osrmcpy import OSRM, Coordinate


# Example User Code
def main():
    if len(sys.argv) != 2:
        sys.exit('Usage: {} monaco.osrm'.format(sys.argv[0]))

    osrm = OSRM(sys.argv[1])

    start = Coordinate(longitude=7.419758, latitude=43.731142)
    end = Coordinate(longitude=7.419505, latitude=43.736825)


    route = osrm.route([start, end])

    if route:
        print('Distance: {0:.0f} meters'.format(route.distance))
        print('Duration: {0:.0f} seconds'.format(route.duration))
    else:
        print('No route found')


    # Somewhere in an area in Monaco..
    bottom_left = Coordinate(longitude=7.413194, latitude=43.731056)
    top_right = Coordinate(longitude=7.421639, latitude=43.735440)

    random_coordinate = lambda: Coordinate(longitude=random.uniform(bottom_left.longitude, top_right.longitude),
                                           latitude=random.uniform(bottom_left.latitude, top_right.latitude))

    table = osrm.table([random_coordinate() for _ in range(10)])

    if table:
        print('Table')
        for row in table:
            for duration in row:
                print('{0:.0f}s\t'.format(duration), end='')
            print()
    else:
        print('No table found')



if __name__ == '__main__':
    main()
