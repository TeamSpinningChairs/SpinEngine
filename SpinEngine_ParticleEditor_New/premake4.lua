
local debug_flags = {
  "Symbols",
  "NoMinimalRebuild",
  "NoEditAndContinue",
  "FloatFast",
  "StaticRuntime",
  "WinMain"
}
local release_flags = {
  "Symbols",
  "NoMinimalRebuild",
  "NoEditAndContinue",
  "FloatFast",
  "Optimize",
  "StaticRuntime",
  "WinMain"
}

local development_flags = {
  "Symbols",
  "NoMinimalRebuild",
  "NoEditAndContinue",
  "FloatFast",
  "StaticRuntime",
  "WinMain"
}

local debug_defines = {
  "_DEBUG",
  "_CRT_SECURE_NO_WARNINGS",
  "TIXML_USE_STL",
  "D3D_DEBUG_INFO"
}
local development_defines = {
  "_DEBUG",
  "_CRT_SECURE_NO_WARNINGS",
  "TIXML_USE_STL",
  "D3D_DEBUG_INFO"
}

local release_defines = {
  "NDEBUG",
  "_CRT_SECURE_NO_WARNINGS",
  "TIXML_USE_STL"
}

local source_dir = "./ParticleEditor/"
local resource_dir = "./Assets/"
local shader_dir = resource_dir .. "shaders/"
local lib_dir = "./ParticleEditor/Libraries/"
local header_dir = { source_dir, source_dir .. "**" }

local sources = { source_dir .. "**.cpp", source_dir .. "**.h"
                  , shader_dir .. "**.fx" -- include shader sources, but need to exclude from build
                  , source_dir .. "**.rc" -- include resource file for changing icon
                }
local math_dir = source_dir .. "Math/"

local libs = { "AntTweakBar", "d3dx9", "d3d9", "fmodex_vc"} --add libs here
local lang = "C++"                  -- language we're using
local loc = "build"                 -- project location
local projKind = "WindowedApp"      -- project kind
local pch_dir = ""--"precompiled/"
local pch_file = ""--"precompiled"

-- project and solution name --
local solution_name = "Spin_ParticleEditor" -- there must not be any space for the os.remove to work properly
local engine_name = "Engine_Cpy"
local math_lib_name = "Math Library"

-- create solution --
solution (solution_name)
configurations { "Debug", "Release"}

  -- engine --
	project (engine_name)
    kind (projKind)
    language (lang)
    files( sources )
    --pchheader( pch_file .. ".h" )
    --pchsource( source_dir .. pch_dir .. pch_file .. ".cpp" )
    
    includedirs { header_dir }
    libdirs(lib_dir)
    links ( libs )
    
    -- note: any exclusion of files must be done before the configuration section --
    
    ------ exlude any unwanted files ------
    
    -- exclude math library test
    --excludes {math_dir .. "MathTest.cpp"}
    
    ------ configure each files behaviour ------
    
    -- configure to exclude shaders from build
    configuration {shader_dir .. "**.fx"}
      buildaction "None"

      
    -- other configurations
    
    configuration "Debug"
      defines(debug_defines)
      flags(debug_flags)
      
    --configuration "Development"
    --  defines(development_defines)
    --  flags(development_flags)
      
    configuration "Release"
      defines(release_defines)
      flags(release_flags)
    
  --------------------------
  
  -- math library --
  --project (math_lib_name)
  --  kind("ConsoleApp")
  --  language(lang)
  --  files( {math_dir .. "**"
  --         ,source_dir .. pch_dir .. "**"  -- precompiled
  --         ,source_dir .. "Utilities/**"   -- utilities
  --         ,source_dir .. "Debug/**"       -- debugger
  --         } ) -- include all files in math library and precompiled file
  --         
  --  libdirs(lib_dir)
  --  links ( libs )
  --  
  --  -- math library actually doesnt need these precompiled header,
  --  -- but since the source code are shared with the engine, it need to include it
  --  pchheader( pch_file .. ".h" )
  --  pchsource( source_dir .. pch_dir .. pch_file .. ".cpp" )
  --  
  --  -- precompiled requires other files as well, 
  --  includedirs { header_dir }
  --  
  --  -- exclude crash handler, dont need it
  --  excludes ({source_dir .. "Debug/CrashHandler.cpp",
  --             source_dir .. "Utilities/Tweening/**"    })
  --  
  --  -- other configurations --
  --  configuration "Debug"
  --    defines(debug_defines)
  --    flags(debug_flags)
  --    
  --  configuration "Release"
  --    defines(release_defines)
  --    flags(release_flags)
      
      
--------------------------------------
      
-- check for actions --
if _ACTION == "clean" then
  
  -- delete working dirs
  os.rmdir( "./ipch") -- remove pch files
  os.rmdir( "./obj" ) -- remove generated obj files
  os.remove( solution_name .. ".sdf") -- remove sdf file
  os.remove( solution_name .. ".v10.suo") -- remove suo file
  os.remove( solution_name .. ".v11.suo") -- remove suo file
  os.remove( solution_name .. ".v12.suo") -- remove suo file
  os.remove( solution_name .. ".v13.suo") -- remove suo file
end


