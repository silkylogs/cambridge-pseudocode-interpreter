workspace "CambridgePseudocodeInterpeter"
   architecture "X86_64"
   configurations {
       "Debug",
       "Test",
    --    "Release"
   }

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "CambridgePseudocodeInterpreter"
   kind "ConsoleApp"
   language "C"
   cdialect "c89"
   warnings "Extra"
   
   targetdir ("bin/"          .. output_dir .. "/%{prj.name}")
   objdir    ("bin-temp-obj/" .. output_dir .. "/%{prj.name}")

   files { "vm.c", "vm.h" }

   filter "configurations:Test"
      files { "test.c" }
      
   filter "configurations:Debug"
      files { "main.c" }

   -- Note: Place your raylib folder inside the third_party directory.
   -- libdirs { "third_party/raylib-5.0_win64_msvc16/lib" }
   -- includedirs { "third_party/raylib-5.0_win64_msvc16/include" }
   -- links { "raylib", "raylibdll", "Gdi32", "shell32", "WinMM" }

   filter "system:windows"
      cdialect "C89"
      -- fatalwarnings {
      --    4020, -- Function has too many arguments
      --    4024, -- Different types for formal and actual parameter
      --    4047, -- T differs in levels of indirection
      --    4131, -- Function uses old-style declarator
      --    4473, -- Function has not enough arguments passed for format string
      --    4131, -- Function uses old-style declarator
      --    4477, -- Mismatched format string and variadic argument type
      --    4052, -- function declarations different; one contains variable arguments
      --    4715, -- not all control paths return a value
      --    4716, -- Function must return a value
      --    4013, -- Function undefined; assuming extern returning int
      -- }
      disablewarnings {
         4777, -- 'printf' : format string
         4820, -- padding added after data member
         5045, -- Compiler will insert Spectre mitigation for memory load
         4100, -- unreferenced formal parameter
         4189, -- local variable is initialized but not referenced
         4101, -- unreferenced local variable
         4996, -- Function is unsafe.
      }
      sanitize { "Address" }

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