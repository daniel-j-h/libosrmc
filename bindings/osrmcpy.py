#!/usr/bin/env python2

from __future__ import print_function, division

import ctypes as c
from collections import namedtuple
from contextlib import contextmanager

# Native Functions Bridge

lib = c.cdll.LoadLibrary('libosrmc.so')


# Config
lib.osrmc_config_construct.restype = c.c_void_p
lib.osrmc_config_construct.argtypes = [c.c_char_p]

lib.osrmc_config_destruct.restype = None
lib.osrmc_config_destruct.argtypes = [c.c_void_p]

# ORM
lib.osrmc_osrm_construct.restype = c.c_void_p
lib.osrmc_osrm_construct.argtypes = [c.c_void_p]

lib.osrmc_osrm_destruct.restype = None
lib.osrmc_osrm_destruct.argtypes = [c.c_void_p]

# Generic Param Handling
lib.osrmc_params_add_coordinate.restype = None
lib.osrmc_params_add_coordinate.argtypes = [c.c_void_p, c.c_float, c.c_float]

# Route Params
lib.osrmc_route_params_construct.restype = c.c_void_p
lib.osrmc_route_params_construct.argtypes = None

lib.osrmc_route_params_destruct.restype = None
lib.osrmc_route_params_destruct.argtypes = [c.c_void_p]

# Route
lib.osrmc_route.restype = c.c_void_p
lib.osrmc_route.argtypes = [c.c_void_p, c.c_void_p]

lib.osrmc_route_response_destruct.restype = None
lib.osrmc_route_response_destruct.argtypes = [c.c_void_p]

lib.osrmc_route_response_distance.restype = c.c_float
lib.osrmc_route_response_distance.argtypes = [c.c_void_p]

lib.osrmc_route_response_duration.restype = c.c_float
lib.osrmc_route_response_duration.argtypes = [c.c_void_p]

# Table Params
lib.osrmc_table_params_construct.restype = c.c_void_p
lib.osrmc_table_params_construct.argtypes = None

lib.osrmc_table_params_destruct.restype = None
lib.osrmc_table_params_destruct.argtypes = [c.c_void_p]

# Table

lib.osrmc_table.restype = c.c_void_p
lib.osrmc_table.argtypes = [c.c_void_p, c.c_void_p]

lib.osrmc_table_response_destruct.restype = None
lib.osrmc_table_response_destruct.argtypes = [c.c_void_p]

lib.osrmc_table_response_duration.restype = c.c_float
lib.osrmc_table_response_duration.argtypes = [c.c_void_p, c.c_ulong, c.c_ulong]


# Python Library Interface

@contextmanager
def scoped_config(base_path):
    config = lib.osrmc_config_construct(base_path)
    yield config
    lib.osrmc_config_destruct(config)

@contextmanager
def scoped_osrm(config):
    osrm = lib.osrmc_osrm_construct(config)
    yield osrm
    lib.osrmc_osrm_destruct(osrm)

@contextmanager
def scoped_route_params():
    params = lib.osrmc_route_params_construct()
    yield params
    lib.osrmc_route_params_destruct(params)

@contextmanager
def scoped_route(osrm, params):
    route = lib.osrmc_route(osrm, params)
    yield route
    lib.osrmc_route_response_destruct(route)

@contextmanager
def scoped_table_params():
    params = lib.osrmc_table_params_construct()
    yield params
    lib.osrmc_table_params_destruct(params)

@contextmanager
def scoped_table(osrm, params):
    route = lib.osrmc_table(osrm, params)
    yield route
    lib.osrmc_table_response_destruct(route)


Coordinate = namedtuple('Coordinate', 'longitude latitude')
Route = namedtuple('Route', 'distance duration')
Table = list


class OSRM:
    def __init__(_, base_path):
        _.config = None
        _.osrm = None

        _.config = lib.osrmc_config_construct(base_path)
        assert _.config

        _.osrm = lib.osrmc_osrm_construct(_.config)
        assert _.osrm

    def __del__(_):
        if _.osrm:
            lib.osrmc_osrm_destruct(_.osrm)
        if _.config:
            lib.osrmc_config_destruct(_.config)

    def route(_, coordinates):
        with scoped_route_params() as params:
            assert params

            for coordinate in coordinates:
                lib.osrmc_params_add_coordinate(params, coordinate.longitude, coordinate.latitude)

            with scoped_route(_.osrm, params) as route:
                if route:
                    distance = lib.osrmc_route_response_distance(route)
                    duration = lib.osrmc_route_response_duration(route)
                    return Route(distance=distance, duration=duration)
                else:
                    return None

    def table(_, coordinates):
        with scoped_table_params() as params:
            assert params

            for coordinate in coordinates:
                lib.osrmc_params_add_coordinate(params, coordinate.longitude, coordinate.latitude)

            with scoped_table(_.osrm, params) as table:
                if table:
                    n = len(coordinates)  # Only symmetric version supported
                    return Table([lib.osrmc_table_response_duration(table, s, t) for t in range(n)] for s in range(n))
                else:
                    return None
