// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	C++ class header boilerplate exported from UnrealHeaderTool.
	This is automatically generated by the tools.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UObject;
class APlayerController;
class ULoginUserCallbackProxy;
#ifdef ADVANCEDSESSIONS_LoginUserCallbackProxy_generated_h
#error "LoginUserCallbackProxy.generated.h already included, missing '#pragma once' in LoginUserCallbackProxy.h"
#endif
#define ADVANCEDSESSIONS_LoginUserCallbackProxy_generated_h

#define FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LoginUserCallbackProxy_h_11_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execLoginUser) \
	{ \
		P_GET_OBJECT(UObject,Z_Param_WorldContextObject); \
		P_GET_OBJECT(APlayerController,Z_Param_PlayerController); \
		P_GET_PROPERTY(UStrProperty,Z_Param_UserID); \
		P_GET_PROPERTY(UStrProperty,Z_Param_UserToken); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(ULoginUserCallbackProxy**)Z_Param__Result=ULoginUserCallbackProxy::LoginUser(Z_Param_WorldContextObject,Z_Param_PlayerController,Z_Param_UserID,Z_Param_UserToken); \
		P_NATIVE_END; \
	}


#define FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LoginUserCallbackProxy_h_11_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execLoginUser) \
	{ \
		P_GET_OBJECT(UObject,Z_Param_WorldContextObject); \
		P_GET_OBJECT(APlayerController,Z_Param_PlayerController); \
		P_GET_PROPERTY(UStrProperty,Z_Param_UserID); \
		P_GET_PROPERTY(UStrProperty,Z_Param_UserToken); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(ULoginUserCallbackProxy**)Z_Param__Result=ULoginUserCallbackProxy::LoginUser(Z_Param_WorldContextObject,Z_Param_PlayerController,Z_Param_UserID,Z_Param_UserToken); \
		P_NATIVE_END; \
	}


#define FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LoginUserCallbackProxy_h_11_INCLASS_NO_PURE_DECLS \
	private: \
	static void StaticRegisterNativesULoginUserCallbackProxy(); \
	friend ADVANCEDSESSIONS_API class UClass* Z_Construct_UClass_ULoginUserCallbackProxy(); \
	public: \
	DECLARE_CLASS(ULoginUserCallbackProxy, UOnlineBlueprintCallProxyBase, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/AdvancedSessions"), ADVANCEDSESSIONS_API) \
	DECLARE_SERIALIZER(ULoginUserCallbackProxy) \
	/** Indicates whether the class is compiled into the engine */ \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LoginUserCallbackProxy_h_11_INCLASS \
	private: \
	static void StaticRegisterNativesULoginUserCallbackProxy(); \
	friend ADVANCEDSESSIONS_API class UClass* Z_Construct_UClass_ULoginUserCallbackProxy(); \
	public: \
	DECLARE_CLASS(ULoginUserCallbackProxy, UOnlineBlueprintCallProxyBase, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/AdvancedSessions"), ADVANCEDSESSIONS_API) \
	DECLARE_SERIALIZER(ULoginUserCallbackProxy) \
	/** Indicates whether the class is compiled into the engine */ \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LoginUserCallbackProxy_h_11_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	ADVANCEDSESSIONS_API ULoginUserCallbackProxy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ULoginUserCallbackProxy) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(ADVANCEDSESSIONS_API, ULoginUserCallbackProxy); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ULoginUserCallbackProxy); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	ADVANCEDSESSIONS_API ULoginUserCallbackProxy(ULoginUserCallbackProxy&&); \
	ADVANCEDSESSIONS_API ULoginUserCallbackProxy(const ULoginUserCallbackProxy&); \
public:


#define FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LoginUserCallbackProxy_h_11_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	ADVANCEDSESSIONS_API ULoginUserCallbackProxy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	ADVANCEDSESSIONS_API ULoginUserCallbackProxy(ULoginUserCallbackProxy&&); \
	ADVANCEDSESSIONS_API ULoginUserCallbackProxy(const ULoginUserCallbackProxy&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(ADVANCEDSESSIONS_API, ULoginUserCallbackProxy); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ULoginUserCallbackProxy); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ULoginUserCallbackProxy)


#define FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LoginUserCallbackProxy_h_11_PRIVATE_PROPERTY_OFFSET
#define FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LoginUserCallbackProxy_h_8_PROLOG
#define FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LoginUserCallbackProxy_h_11_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LoginUserCallbackProxy_h_11_PRIVATE_PROPERTY_OFFSET \
	FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LoginUserCallbackProxy_h_11_RPC_WRAPPERS \
	FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LoginUserCallbackProxy_h_11_INCLASS \
	FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LoginUserCallbackProxy_h_11_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LoginUserCallbackProxy_h_11_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LoginUserCallbackProxy_h_11_PRIVATE_PROPERTY_OFFSET \
	FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LoginUserCallbackProxy_h_11_RPC_WRAPPERS_NO_PURE_DECLS \
	FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LoginUserCallbackProxy_h_11_INCLASS_NO_PURE_DECLS \
	FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LoginUserCallbackProxy_h_11_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class LoginUserCallbackProxy."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LoginUserCallbackProxy_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
