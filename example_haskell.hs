{-# LANGUAGE ForeignFunctionInterface #-}
{-# LANGUAGE OverloadedStrings #-}
module Main where

import Foreign.C
import Foreign.Ptr

import Data.Text (Text, unpack)
import Data.Monoid ((<>))
import Control.Exception (bracket)
import Control.Monad (when)


-- Opaque Types

newtype Config = Config { getConfig :: Ptr Config }
newtype OSRM = OSRM { getOSRM :: Ptr OSRM }
newtype RouteParams = RouteParams { getRouteParams :: Ptr RouteParams }
newtype RouteResponse = RouteResponse { getRouteResponse :: Ptr RouteResponse }

data Coordinate = Coordinate { longitude :: Float
                             , latitude  :: Float} deriving (Show)


-- ABI Stability

foreign import ccall "osrmc_get_version"
    getVersion :: CUInt

foreign import ccall "osrmc_is_abi_compatible"
    isABICompatible :: CInt

-- API

foreign import ccall "osrmc_config_construct"
    constructConfig :: CString -> IO Config

foreign import ccall "osrmc_config_destruct"
    destructConfig :: Config -> IO ()


foreign import ccall "osrmc_osrm_construct"
    constructOSRM :: Config -> IO OSRM

foreign import ccall "osrmc_osrm_destruct"
    destructOSRM :: OSRM -> IO ()


foreign import ccall "osrmc_route_params_construct"
    constructRouteParams :: IO RouteParams

foreign import ccall "osrmc_route_params_destruct"
    destructRouteParams :: RouteParams -> IO ()

foreign import ccall "osrmc_route_params_add_coordinate"
    addCoordinate :: RouteParams -> CFloat -> CFloat -> IO ()


foreign import ccall "osrmc_route"
    route :: OSRM -> RouteParams -> IO RouteResponse

foreign import ccall "osrmc_route_response_destruct"
    destructRouteResponse :: RouteResponse -> IO ()

foreign import ccall "osrmc_route_response_distance"
    distance :: RouteResponse -> IO CFloat

foreign import ccall "osrmc_route_response_duration"
    duration :: RouteResponse -> IO CFloat

withConfig :: Text -> (Config -> IO ()) -> IO ()
withConfig basePath body = bracket construct destruct body
  where construct = withCString (unpack basePath) $ constructConfig
        destruct  = destructConfig

withOSRM :: Config -> (OSRM -> IO ()) -> IO ()
withOSRM config body = bracket construct destruct body
  where construct = constructOSRM config
        destruct  = destructOSRM

withRouteParams :: (RouteParams -> IO ()) -> IO ()
withRouteParams body = bracket construct destruct body
  where construct = constructRouteParams
        destruct  = destructRouteParams

withRoute :: OSRM -> RouteParams -> (RouteResponse -> IO ()) -> IO ()
withRoute osrm params body = bracket construct destruct body
  where construct = route osrm params
        destruct  = destructRouteResponse


-- TODO(daniel-j-h):
--  - Error handling, rip out fail
--  - Eliminate multiple level bracketing, continuation monad (?)
--  - Nice abstractions, example below is ugly and imperative as it's using low level primitives
--  - Hints: EitherT, ConT, ResourceT


main = do
    let base  = "/tmp/osrm-backend/test/data/monaco.osrm"

    let start = Coordinate{ longitude=7.419758, latitude=43.731142 }
    let end   = Coordinate{ longitude=7.419505, latitude=43.736825 }

    withConfig base $ \config -> do
        when ((getConfig config) == nullPtr) $ fail "config"

        withOSRM config $ \osrm -> do
            when ((getOSRM osrm) == nullPtr) $ fail "osrm"

            withRouteParams $ \params -> do
                when ((getRouteParams params) == nullPtr) $ fail "params"

                addCoordinate params (CFloat . longitude $ start) (CFloat . latitude $ start)
                addCoordinate params (CFloat . longitude $ end)   (CFloat . latitude $ end)

                withRoute osrm params $ \response -> do
                    when ((getRouteResponse response) == nullPtr) $ fail "response"

                    dist <- distance response
                    dura <- duration response

                    putStrLn $ "Distance: " <> (show dist) <> " meters"
                    putStrLn $ "Duration: " <> (show dist) <> " seconds"
