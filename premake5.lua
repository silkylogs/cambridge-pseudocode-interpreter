workspace "CambridgePseudocodeInterpeter"
   architecture "X86_64"
   configurations {
       "Debug",
       "Release",
   }

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "CambridgePseudocodeInterpreter"
   kind "ConsoleApp"
   language "C"
   cdialect "c89"
   warnings "Everything"
   
   targetdir ("bin/"          .. output_dir .. "/%{prj.name}")
   objdir    ("bin-temp-obj/" .. output_dir .. "/%{prj.name}")   
   files {
       "main.c"
   } 

   filter "system:windows"
       warnings "Everything"
       cdialect "c89"

   filter "system:linux"
   -- Note: to build with clang: `premake5 gmake2 --os=linux --cc=clang`
       warnings "Extra"
       --fatalwarnings {
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
       --}  

   filter "configurations:Debug"
       symbols "On"
       
   filter "configurations:Release"
       optimize "On"

newaction {
   trigger     = "clean-build",
   description = "Removes the build file and its contents",
   execute     = function ()
      print("Cleaning...")
      os.rmdir("./bin")
      print("Done.")
   end
}