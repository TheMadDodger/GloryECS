workspace "GloryECS"
	platforms {
		"Win32",
		"x64"
	}

	startproject "Demo"
	startproject "GloryECSStatic"
	startproject "GloryReflectStatic"

	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

leak_detector = "C:/Program Files (x86)/Visual Leak Detector/include"
lib_outdir = "../Build/Lib/%{cfg.buildcfg}/%{cfg.platform}"
exe_outdir = "../Build/Demo/%{cfg.buildcfg}/%{cfg.platform}"

include "Demo"
include "GloryECSStatic"
include "GloryReflectStatic"
