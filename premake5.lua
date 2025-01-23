workspace "CambridgePseudocodeInterpeter"
   architecture "X86_64"
   configurations {
       "Debug",
    --    "Release"
   }

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "CambridgePseudocodeInterpreter"
   kind "ConsoleApp"
   language "C"
   cdialect "c99"
   warnings "High"
   
   targetdir ("bin/"          .. output_dir .. "/%{prj.name}")
   objdir    ("bin-temp-obj/" .. output_dir .. "/%{prj.name}")
   files { "main.c" }
   -- Note: Place your raylib folder inside the third_party directory.
   libdirs { "third_party/raylib-5.0_win64_msvc16/lib" }
   includedirs { "third_party/raylib-5.0_win64_msvc16/include" }
   links { "raylib", "raylibdll", "Gdi32", "shell32", "WinMM" }

   filter "system:windows"
       warnings "Extra"
       cdialect "C99"

   filter "system:linux"
   -- Note: to build with clang: `premake5 gmake2 --os=linux --cc=clang`
       warnings "Extra"
       fatalwarnings {
          "write-strings",
          "strict-prototypes",
          "missing-prototypes",
          "missing-declarations",
          "sign-compare",
          "logical-op",
          "type-limits",
          "suggest-attribute=pure",
          "suggest-attribute=const",
          "suggest-attribute=noreturn",
          "suggest-attribute=format",
          "format-nonliteral",
          "error=vla",
       }  

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