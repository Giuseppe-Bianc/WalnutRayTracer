project "RayTracer"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "../Walnut/vendor/imgui",
      "../Walnut/vendor/glfw/include",
      "../Walnut/vendor/glm",
      "../Walnut/Walnut/src",
      "external/spdlog/include",
      "external/opencv/build/include",
      "%{IncludeDir.VulkanSDK}",
   }

   libdirs {
      "external/opencv/build/x64/vc16/lib"  -- Replace with the path to your OpenCV library directory
   }


   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "WL_PLATFORM_WINDOWS" }

   filter "configurations:Debug"
      defines { "WL_DEBUG" }
      runtime "Debug"
      inlining "Auto"
      symbols "On"
      links {
         "Walnut",
         "opencv_world470d",
      }

   filter "configurations:Release"
      defines { "WL_RELEASE" }
      runtime "Release"
      optimize "On"
      inlining "Auto"
      symbols "On"
      flags { "LinkTimeOptimization", "MultiProcessorCompile"} 
      links {
         "Walnut",
         "opencv_world470",
      }

   filter "configurations:Dist"
      kind "WindowedApp"
      defines { "WL_DIST" }
      runtime "Release"
      optimize "On"
      inlining "Auto"
      symbols "Off"
      omitframepointer "On"
      flags { "LinkTimeOptimization", "MultiProcessorCompile", "NoBufferSecurityCheck", "NoIncrementalLink" }
      links {
         "Walnut",
         "opencv_world470",
      }