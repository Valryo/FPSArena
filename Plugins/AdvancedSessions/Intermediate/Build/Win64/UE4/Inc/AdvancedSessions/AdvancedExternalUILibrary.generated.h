// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	C++ class header boilerplate exported from UnrealHeaderTool.
	This is automatically generated by the tools.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

PRAGMA_DISABLE_DEPRECATION_WARNINGS
struct FBPUniqueNetId;
enum class EBlueprintResultSwitch : uint8;
class APlayerController;
#ifdef ADVANCEDSESSIONS_AdvancedExternalUILibrary_generated_h
#error "AdvancedExternalUILibrary.generated.h already included, missing '#pragma once' in AdvancedExternalUILibrary.h"
#endif
#define ADVANCEDSESSIONS_AdvancedExternalUILibrary_generated_h

#define FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_27_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execShowAccountUpgradeUI) \
	{ \
		P_GET_STRUCT(FBPUniqueNetId,Z_Param_PlayerRequestingAccountUpgradeUI); \
		P_GET_ENUM_REF(EBlueprintResultSwitch,Z_Param_Out_Result); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UAdvancedExternalUILibrary::ShowAccountUpgradeUI(Z_Param_PlayerRequestingAccountUpgradeUI,(EBlueprintResultSwitch&)(Z_Param_Out_Result)); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execShowProfileUI) \
	{ \
		P_GET_STRUCT(FBPUniqueNetId,Z_Param_PlayerViewingProfile); \
		P_GET_STRUCT(FBPUniqueNetId,Z_Param_PlayerToViewProfileOf); \
		P_GET_ENUM_REF(EBlueprintResultSwitch,Z_Param_Out_Result); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UAdvancedExternalUILibrary::ShowProfileUI(Z_Param_PlayerViewingProfile,Z_Param_PlayerToViewProfileOf,(EBlueprintResultSwitch&)(Z_Param_Out_Result)); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execCloseWebURLUI) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UAdvancedExternalUILibrary::CloseWebURLUI(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execShowWebURLUI) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_URLToShow); \
		P_GET_ENUM_REF(EBlueprintResultSwitch,Z_Param_Out_Result); \
		P_GET_TARRAY_REF(FString,Z_Param_Out_AllowedDomains); \
		P_GET_UBOOL(Z_Param_bEmbedded); \
		P_GET_UBOOL(Z_Param_bShowBackground); \
		P_GET_UBOOL(Z_Param_bShowCloseButton); \
		P_GET_PROPERTY(UIntProperty,Z_Param_OffsetX); \
		P_GET_PROPERTY(UIntProperty,Z_Param_OffsetY); \
		P_GET_PROPERTY(UIntProperty,Z_Param_SizeX); \
		P_GET_PROPERTY(UIntProperty,Z_Param_SizeY); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UAdvancedExternalUILibrary::ShowWebURLUI(Z_Param_URLToShow,(EBlueprintResultSwitch&)(Z_Param_Out_Result),Z_Param_Out_AllowedDomains,Z_Param_bEmbedded,Z_Param_bShowBackground,Z_Param_bShowCloseButton,Z_Param_OffsetX,Z_Param_OffsetY,Z_Param_SizeX,Z_Param_SizeY); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execShowLeaderBoardUI) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_LeaderboardName); \
		P_GET_ENUM_REF(EBlueprintResultSwitch,Z_Param_Out_Result); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UAdvancedExternalUILibrary::ShowLeaderBoardUI(Z_Param_LeaderboardName,(EBlueprintResultSwitch&)(Z_Param_Out_Result)); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execShowInviteUI) \
	{ \
		P_GET_OBJECT(APlayerController,Z_Param_PlayerController); \
		P_GET_ENUM_REF(EBlueprintResultSwitch,Z_Param_Out_Result); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UAdvancedExternalUILibrary::ShowInviteUI(Z_Param_PlayerController,(EBlueprintResultSwitch&)(Z_Param_Out_Result)); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execShowFriendsUI) \
	{ \
		P_GET_OBJECT(APlayerController,Z_Param_PlayerController); \
		P_GET_ENUM_REF(EBlueprintResultSwitch,Z_Param_Out_Result); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UAdvancedExternalUILibrary::ShowFriendsUI(Z_Param_PlayerController,(EBlueprintResultSwitch&)(Z_Param_Out_Result)); \
		P_NATIVE_END; \
	}


#define FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_27_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execShowAccountUpgradeUI) \
	{ \
		P_GET_STRUCT(FBPUniqueNetId,Z_Param_PlayerRequestingAccountUpgradeUI); \
		P_GET_ENUM_REF(EBlueprintResultSwitch,Z_Param_Out_Result); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UAdvancedExternalUILibrary::ShowAccountUpgradeUI(Z_Param_PlayerRequestingAccountUpgradeUI,(EBlueprintResultSwitch&)(Z_Param_Out_Result)); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execShowProfileUI) \
	{ \
		P_GET_STRUCT(FBPUniqueNetId,Z_Param_PlayerViewingProfile); \
		P_GET_STRUCT(FBPUniqueNetId,Z_Param_PlayerToViewProfileOf); \
		P_GET_ENUM_REF(EBlueprintResultSwitch,Z_Param_Out_Result); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UAdvancedExternalUILibrary::ShowProfileUI(Z_Param_PlayerViewingProfile,Z_Param_PlayerToViewProfileOf,(EBlueprintResultSwitch&)(Z_Param_Out_Result)); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execCloseWebURLUI) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UAdvancedExternalUILibrary::CloseWebURLUI(); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execShowWebURLUI) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_URLToShow); \
		P_GET_ENUM_REF(EBlueprintResultSwitch,Z_Param_Out_Result); \
		P_GET_TARRAY_REF(FString,Z_Param_Out_AllowedDomains); \
		P_GET_UBOOL(Z_Param_bEmbedded); \
		P_GET_UBOOL(Z_Param_bShowBackground); \
		P_GET_UBOOL(Z_Param_bShowCloseButton); \
		P_GET_PROPERTY(UIntProperty,Z_Param_OffsetX); \
		P_GET_PROPERTY(UIntProperty,Z_Param_OffsetY); \
		P_GET_PROPERTY(UIntProperty,Z_Param_SizeX); \
		P_GET_PROPERTY(UIntProperty,Z_Param_SizeY); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UAdvancedExternalUILibrary::ShowWebURLUI(Z_Param_URLToShow,(EBlueprintResultSwitch&)(Z_Param_Out_Result),Z_Param_Out_AllowedDomains,Z_Param_bEmbedded,Z_Param_bShowBackground,Z_Param_bShowCloseButton,Z_Param_OffsetX,Z_Param_OffsetY,Z_Param_SizeX,Z_Param_SizeY); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execShowLeaderBoardUI) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_LeaderboardName); \
		P_GET_ENUM_REF(EBlueprintResultSwitch,Z_Param_Out_Result); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UAdvancedExternalUILibrary::ShowLeaderBoardUI(Z_Param_LeaderboardName,(EBlueprintResultSwitch&)(Z_Param_Out_Result)); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execShowInviteUI) \
	{ \
		P_GET_OBJECT(APlayerController,Z_Param_PlayerController); \
		P_GET_ENUM_REF(EBlueprintResultSwitch,Z_Param_Out_Result); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UAdvancedExternalUILibrary::ShowInviteUI(Z_Param_PlayerController,(EBlueprintResultSwitch&)(Z_Param_Out_Result)); \
		P_NATIVE_END; \
	} \
 \
	DECLARE_FUNCTION(execShowFriendsUI) \
	{ \
		P_GET_OBJECT(APlayerController,Z_Param_PlayerController); \
		P_GET_ENUM_REF(EBlueprintResultSwitch,Z_Param_Out_Result); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UAdvancedExternalUILibrary::ShowFriendsUI(Z_Param_PlayerController,(EBlueprintResultSwitch&)(Z_Param_Out_Result)); \
		P_NATIVE_END; \
	}


#define FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_27_INCLASS_NO_PURE_DECLS \
	private: \
	static void StaticRegisterNativesUAdvancedExternalUILibrary(); \
	friend ADVANCEDSESSIONS_API class UClass* Z_Construct_UClass_UAdvancedExternalUILibrary(); \
	public: \
	DECLARE_CLASS(UAdvancedExternalUILibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/AdvancedSessions"), NO_API) \
	DECLARE_SERIALIZER(UAdvancedExternalUILibrary) \
	/** Indicates whether the class is compiled into the engine */ \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_27_INCLASS \
	private: \
	static void StaticRegisterNativesUAdvancedExternalUILibrary(); \
	friend ADVANCEDSESSIONS_API class UClass* Z_Construct_UClass_UAdvancedExternalUILibrary(); \
	public: \
	DECLARE_CLASS(UAdvancedExternalUILibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/AdvancedSessions"), NO_API) \
	DECLARE_SERIALIZER(UAdvancedExternalUILibrary) \
	/** Indicates whether the class is compiled into the engine */ \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_27_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UAdvancedExternalUILibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UAdvancedExternalUILibrary) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UAdvancedExternalUILibrary); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UAdvancedExternalUILibrary); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UAdvancedExternalUILibrary(UAdvancedExternalUILibrary&&); \
	NO_API UAdvancedExternalUILibrary(const UAdvancedExternalUILibrary&); \
public:


#define FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_27_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UAdvancedExternalUILibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UAdvancedExternalUILibrary(UAdvancedExternalUILibrary&&); \
	NO_API UAdvancedExternalUILibrary(const UAdvancedExternalUILibrary&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UAdvancedExternalUILibrary); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UAdvancedExternalUILibrary); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UAdvancedExternalUILibrary)


#define FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_27_PRIVATE_PROPERTY_OFFSET
#define FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_24_PROLOG
#define FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_27_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_27_PRIVATE_PROPERTY_OFFSET \
	FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_27_RPC_WRAPPERS \
	FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_27_INCLASS \
	FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_27_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_27_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_27_PRIVATE_PROPERTY_OFFSET \
	FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_27_RPC_WRAPPERS_NO_PURE_DECLS \
	FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_27_INCLASS_NO_PURE_DECLS \
	FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_27_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FPSArena_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS