LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := dobby

LOCAL_CPPFLAGS += -std=c++11 -DDOBBY_LOGGING_DISABLE -DNearBranch

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH) \
    $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/source \
    $(LOCAL_PATH)/source/dobby \
    $(LOCAL_PATH)/external \
    $(LOCAL_PATH)/external/logging \
    $(LOCAL_PATH)/builtin-plugin \
    $(LOCAL_PATH)/builtin-plugin/SymbolResolver

LOCAL_SRC_FILES += \
    source/core/arch/CpuFeature.cc \
    source/core/arch/CpuRegister.cc

LOCAL_SRC_FILES += \
    source/core/assembler/assembler.cc \
    source/core/assembler/assembler-arm.cc \
    source/core/assembler/assembler-arm64.cc

LOCAL_SRC_FILES += \
    source/core/codegen/codegen-arm.cc \
    source/core/codegen/codegen-arm64.cc

LOCAL_SRC_FILES += \
    source/MemoryAllocator/CodeBuffer/CodeBufferBase.cc \
    source/MemoryAllocator/AssemblyCodeBuilder.cc \
    source/MemoryAllocator/MemoryAllocator.cc

LOCAL_SRC_FILES += \
    source/InstructionRelocation/arm/InstructionRelocationARM.cc \
    source/InstructionRelocation/arm64/InstructionRelocationARM64.cc \
    source/InstructionRelocation/x86/InstructionRelocationX86.cc \
    source/InstructionRelocation/x86/InstructionRelocationX86Shared.cc \
    source/InstructionRelocation/x86/x86_insn_decode/x86_insn_decode.c

LOCAL_SRC_FILES += \
    source/InterceptRouting/InterceptRouting.cpp

LOCAL_SRC_FILES += \
    source/TrampolineBridge/Trampoline/arm/trampoline_arm.cc \
    source/TrampolineBridge/Trampoline/arm64/trampoline_arm64.cc

LOCAL_SRC_FILES += \
    source/TrampolineBridge/ClosureTrampolineBridge/common_bridge_handler.cc \
    source/TrampolineBridge/ClosureTrampolineBridge/arm/helper_arm.cc \
    source/TrampolineBridge/ClosureTrampolineBridge/arm/closure_bridge_arm.cc \
    source/TrampolineBridge/ClosureTrampolineBridge/arm/ClosureTrampolineARM.cc \
    source/TrampolineBridge/ClosureTrampolineBridge/arm64/helper_arm64.cc \
    source/TrampolineBridge/ClosureTrampolineBridge/arm64/closure_bridge_arm64.cc \
    source/TrampolineBridge/ClosureTrampolineBridge/arm64/ClosureTrampolineARM64.cc

LOCAL_SRC_FILES += \
    source/InterceptRouting/Routing/InstructionInstrument/InstructionInstrument.cc \
    source/InterceptRouting/Routing/InstructionInstrument/RoutingImpl.cc \
    source/InterceptRouting/Routing/InstructionInstrument/instrument_routing_handler.cc

LOCAL_SRC_FILES += \
    source/InterceptRouting/Routing/FunctionInlineHook/FunctionInlineHook.cc \
    source/InterceptRouting/Routing/FunctionInlineHook/RoutingImpl.cc
LOCAL_SRC_FILES += \
    source/InterceptRouting/RoutingPlugin/RoutingPlugin.cc

LOCAL_SRC_FILES += \
    source/InterceptRouting/RoutingPlugin/NearBranchTrampoline/near_trampoline_arm64.cc \
    source/InterceptRouting/RoutingPlugin/NearBranchTrampoline/NearBranchTrampoline.cc \
    source/MemoryAllocator/NearMemoryAllocator.cc
LOCAL_SRC_FILES += \
    source/Backend/UserMode/PlatformUtil/Linux/ProcessRuntimeUtility.cc \
    source/Backend/UserMode/UnifiedInterface/platform-posix.cc \
    source/Backend/UserMode/ExecMemory/code-patch-tool-posix.cc \
    source/Backend/UserMode/ExecMemory/clear-cache-tool-all.c

LOCAL_SRC_FILES += \
    builtin-plugin/SymbolResolver/elf/dobby_symbol_resolver.cc

LOCAL_SRC_FILES += \
    source/dobby.cpp \
    source/Interceptor.cpp \
    source/InterceptEntry.cpp

include $(BUILD_STATIC_LIBRARY)
