-- premake5.lua
   workspace "CambridgePseudocodeInterpeter"
   configurations { "linux-clang", "windows", "linux-gcc" }

project "CambridgePseudocodeInterpreter"
   kind "ConsoleApp"
   language "C"
   cdialect "c99"
   targetdir "bin/%{cfg.buildcfg}"
   architecture "X86_64"   
   -- The other files are #included with main, for now
   files { "main.c" } 


filter "configurations:linux-clang"
   toolset "clang"
   warnings "Everything"

filter "configurations:linux-gcc"
   toolset "gcc"
   warnings "Extra"

filter "configurations:windows"
   toolset "msc"
   warnings "Everything"


newaction {
   trigger     = "clean-build",
   description = "Removes the build file and its contents",
   execute     = function ()
      print("Cleaning...")
      os.rmdir("./bin")
      print("Done.")
   end
}

newaction {
   trigger     = "show-configs",
   description = "Shows the configurations available",
   execute     = function ()
      print("Configurations available: ")
      --for i,line in ipairs(configurations) do
      --    print(i,line)
      --end
      print("for now, please check the build script for available configs")
   end
}

-- TODO: make the compilers heed these warnings
--       backported from the old build script
--   "-Wwrite-strings"
--   "-Wstrict-prototypes"
--   "-Wmissing-prototypes"
--   "-Wmissing-declarations"
--   "-Wsign-compare"
--   "-Wlogical-op"
--   "-Wtype-limits"
--   "-Wsuggest-attribute=pure"
--   "-Wsuggest-attribute=const"
--   "-Wsuggest-attribute=noreturn"
--   "-Wsuggest-attribute=format"
--   "-Wformat-nonliteral"
--   "-fdiagnostics-color=never"
--   "-Werror=vla"