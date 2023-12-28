-- premake5.lua
   workspace "CambridgePseudocodeInterpeter"
   configurations { "All" }

project "CambridgePseudocodeInterpreter"
   kind "ConsoleApp"
   language "C"
   cdialect "c17"
   targetdir "bin/%{cfg.buildcfg}"
   files { "main.c" }

filter "configurations:All"
   defines { "" }
   symbols "On"
   architecture "X86_64"
   warnings "Everything"

   -- Named warnings are applicable for non-msvc compilers
   --fatalwarnings { "-Wwrite-strings" }
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
   --}

newaction {
   trigger     = "clean-build",
   description = "Removes the build file and its contents",
   execute     = function ()
      print("Cleaning...")
      os.rmdir("./bin")
      print("Done.")
   end
}