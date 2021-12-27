#pragma once

#include <string>

#include <jni.h>
#include <android/log.h>

// I fucking hate everything.

// This is used to pass literally nothing to a parameter
#define NOTHING

// Log Function

#define LOG_JNI_TEST(objectToTest, successMessage, failMessage) \
if (objectToTest == nullptr) { __android_log_print(ANDROID_LOG_ERROR, "modloader-utils [JNI]", failMessage, ""#objectToTest); } \
else { __android_log_print(ANDROID_LOG_VERBOSE, "modloader-utils [JNI]", successMessage, ""#objectToTest); }

#define LOG_JNI(message, ...) \
__android_log_print(ANDROID_LOG_ERROR, "modloader-utils [JNI]", message __VA_OPT__(,) __VA_ARGS__)

// New Object

#define NEW_JOBJECT(env, varName, clazz, sig, ...) \
jobject varName; { \
jmethodID GET_JMETHODID(env, varName##_MethodID, clazz, "<init>", sig); \
varName = CALL_METHOD_FROM_JMETHODID(env, varName, clazz, NewObject, varName##_MethodID, __VA_ARGS__ ); }

// Create Global Object
#define CREATE_GLOBAL_JOBJECT(env, varName, object) \
jobject varName = env->NewGlobalRef(object); \
LOG_JNI_TEST(varName, "Created Global \"%s\"", "Failed To Create Global \"%s\"")

// -- Get Methods --

// Get Class

#define GET_JCLASS(env, varName, classSig) \
jclass varName = env->FindClass(classSig); \
LOG_JNI_TEST(varName, "Found Class \"%s\"", "Failed To Find Class \"%s\"")

#define GET_JCLASS_FROM_JOBJECT(env, varName, object) \
jclass varName = env->GetObjectClass(object); \
LOG_JNI_TEST(varName, "Got Object \"%s\"", "Failed To Get Object \"%s\"")

// Get MethodID

#define GET_JMETHODID(env, varName, clazz, methodName, sig) \
varName = env->GetMethodID(clazz, methodName, sig); \
LOG_JNI_TEST(varName, "Got MethodID \"%s\"", "Failed To Get MethodID \"%s\"");

#define GET_STATIC_JMETHODID(env, varName, clazz, methodName, sig) \
varName = env->GetStaticMethodID(clazz, methodName, sig); \
LOG_JNI_TEST(varName, "Got Static MethodID \"%s\"", "Failed To Get Static MethodID \"%s\"")

// Get FieldID

#define GET_JFIELDID(env, varName, clazz, fieldName, sig) \
varName = env->GetFieldID(clazz, fieldName, sig); \
LOG_JNI_TEST(varName, "Got FieldID \"%s\"", "Failed To Get FieldID \"%s\"")

#define GET_STATIC_JFIELDID(env, varName, clazz, fieldName, sig) \
varName = env->GetStaticFieldID(clazz, fieldName, sig); \
LOG_JNI_TEST(varName, "Got Static FieldID \"%s\"", "Failed To Get Static FieldID \"%s\"")

// -- Call Methods -- 

// Call Method From MethodID

#define CALL_METHOD_FROM_JMETHODID(env, varName, object, jniMethod, methodID, ...) \
env->jniMethod(object, methodID __VA_OPT__(,) __VA_ARGS__); \
LOG_JNI("Called \"%s\" On Object \"%s\"", ""#methodID, ""#varName)

#define CALL_METHOD_FROM_JMETHODID_WITHOUT_LOG(env, object, jniMethod, methodID, ...) \
env->jniMethod(object, methodID __VA_OPT__(,) __VA_ARGS__)

// Generic Call Methods

#define CALL_METHOD_GENERIC(env, varName, object, methodName, sig, type, jniMethod, ...) \
type varName; { \
GET_JCLASS_FROM_JOBJECT(env, varName##_Class, object); \
jmethodID GET_JMETHODID(env, varName##_MethodID, varName##_Class, methodName, sig); \
varName = (type)CALL_METHOD_FROM_JMETHODID(env, varName, object, jniMethod, varName##_MethodID, __VA_ARGS__ ); }

#define CALL_STATIC_METHOD_GENERIC(env, varName, clazz, methodName, sig, type, jniMethod, ...) \
type varName; { \
jmethodID GET_STATIC_JMETHODID(env, varName##_MethodID, clazz, methodName, sig); \
varName = (type)CALL_METHOD_FROM_JMETHODID(env, varName, clazz, jniMethod, varName##_MethodID, __VA_ARGS__ ); }

// Call Void Methods

#define CALL_VOID_METHOD(env, object, methodName, sig, ...) { \
GET_JCLASS_FROM_JOBJECT(env, methodName##_Class, object); \
jmethodID GET_JMETHODID(env, methodName##_MethodID, methodName##_Class, ""#methodName, sig); \
CALL_METHOD_FROM_JMETHODID_WITHOUT_LOG(env, object, CallVoidMethod, methodName##_MethodID, __VA_ARGS__ ); }

#define CALL_STATIC_VOID_METHOD(env, clazz, methodName, sig, ...) { \
jmethodID GET_STATIC_JMETHODID(env, methodName##_MethodID, clazz, ""#methodName, sig); \
CALL_METHOD_FROM_JMETHODID_WITHOUT_LOG(env, clazz, CallStaticVoidMethod, methodName##_MethodID, __VA_ARGS__ ); }

// Call Boolean Methods

#define CALL_JBOOLEAN_METHOD(env, varName, object, methodName, sig, ...) \
CALL_METHOD_GENERIC(env, varName, object, methodName, sig, jboolean, CallBooleanMethod, __VA_ARGS__)

#define CALL_STATIC_JBOOLEAN_METHOD(env, varName, clazz, methodName, sig, ...) \
CALL_STATIC_METHOD_GENERIC(env, varName, clazz, methodName, sig, jboolean, CallStaticBooleanMethod, __VA_ARGS__)

// Call Byte Methods

#define CALL_JBYTE_METHOD(env, varName, object, methodName, sig, ...) \
CALL_METHOD_GENERIC(env, varName, object, methodName, sig, jbyte, CallByteMethod, __VA_ARGS__)

#define CALL_STATIC_JBYTE_METHOD(env, varName, clazz, methodName, sig, ...) \
CALL_STATIC_METHOD_GENERIC(env, varName, clazz, methodName, sig, jbyte, CallStaticByteMethod, __VA_ARGS__)

// Call Char Methods

#define CALL_JCHAR_METHOD(env, varName, object, methodName, sig, ...) \
CALL_METHOD_GENERIC(env, varName, object, methodName, sig, jchar, CallCharMethod, __VA_ARGS__)

#define CALL_STATIC_JCHAR_METHOD(env, varName, clazz, methodName, sig, ...) \
CALL_STATIC_METHOD_GENERIC(env, varName, clazz, methodName, sig, jchar, CallStaticCharMethod, __VA_ARGS__)

// Call Double Methods

#define CALL_JDOUBLE_METHOD(env, varName, object, methodName, sig, ...) \
CALL_METHOD_GENERIC(env, varName, object, methodName, sig, jdouble, CallDoubleMethod, __VA_ARGS__)

#define CALL_STATIC_JDOUBLE_METHOD(env, varName, clazz, methodName, sig, ...) \
CALL_STATIC_METHOD_GENERIC(env, varName, clazz, methodName, sig, jdouble, CallStaticDoubleMethod, __VA_ARGS__)

// Call Float Methods

#define CALL_JFLOAT_METHOD(env, varName, object, methodName, sig, ...) \
CALL_METHOD_GENERIC(env, varName, object, methodName, sig, jfloat, CallFloatMethod, __VA_ARGS__)

#define CALL_STATIC_JFLOAT_METHOD(env, varName, clazz, methodName, sig, ...) \
CALL_STATIC_METHOD_GENERIC(env, varName, clazz, methodName, sig, jfloat, CallStaticFloatMethod, __VA_ARGS__)

// Call Int Methods

#define CALL_JINT_METHOD(env, varName, object, methodName, sig, ...) \
CALL_METHOD_GENERIC(env, varName, object, methodName, sig, jint, CallIntMethod, __VA_ARGS__)

#define CALL_STATIC_JINT_METHOD(env, varName, clazz, methodName, sig, ...) \
CALL_STATIC_METHOD_GENERIC(env, varName, clazz, methodName, sig, jint, CallStaticIntMethod, __VA_ARGS__)

// Call Long Methods

#define CALL_JLONG_METHOD(env, varName, object, methodName, sig, ...) \
CALL_METHOD_GENERIC(env, varName, object, methodName, sig, jlong, CallLongMethod, __VA_ARGS__)

#define CALL_STATIC_JLONG_METHOD(env, varName, clazz, methodName, sig, ...) \
CALL_STATIC_METHOD_GENERIC(env, varName, clazz, methodName, sig, jlong, CallStaticLongMethod, __VA_ARGS__)

// Call Object Methods

#define CALL_JOBJECT_METHOD(env, varName, object, methodName, sig, ...) \
CALL_METHOD_GENERIC(env, varName, object, methodName, sig, jobject, CallObjectMethod, __VA_ARGS__)

#define CALL_STATIC_JOBJECT_METHOD(env, varName, clazz, methodName, sig, ...) \
CALL_STATIC_METHOD_GENERIC(env, varName, clazz, methodName, sig, jobject, CallStaticObjectMethod, __VA_ARGS__)

// Call Short Methods

#define CALL_JSHORT_METHOD(env, varName, object, methodName, sig, ...) \
CALL_METHOD_GENERIC(env, varName, object, methodName, sig, jshort, CallShortMethod, __VA_ARGS__)

#define CALL_STATIC_JSHORT_METHOD(env, varName, clazz, methodName, sig, ...) \
CALL_STATIC_METHOD_GENERIC(env, varName, clazz, methodName, sig, jshort, CallStaticShortMethod, __VA_ARGS__)

// Call String Method

#define CALL_JSTRING_METHOD(env, varName, object, methodName, sig, ...) \
CALL_METHOD_GENERIC(env, varName, object, methodName, sig, jstring, CallObjectMethod, __VA_ARGS__)

#define CALL_STATIC_JSTRING_METHOD(env, varName, clazz, methodName, sig, ...) \
CALL_STATIC_METHOD_GENERIC(env, varName, clazz, methodName, sig, jstring, CallStaticObjectMethod, __VA_ARGS__)

// -- Get Field Methods -- 

// Get Field From FieldID

#define GET_JFIELD_FROM_JFIELDID(env, varName, object, fieldID, type, jniMethod) \
(type)env->jniMethod(object, fieldID); \
LOG_JNI_TEST(varName, "Got Field \"%s\"", "Failed To Get Field \"%s\"")

// Get Generic Field

#define GET_FIELD_GENERIC(env, varName, object, clazz, fieldName, sig, type, jniMethod) \
type varName; { \
jfieldID GET_JFIELDID(env, varName##_FieldID, clazz, fieldName, sig); \
varName = GET_JFIELD_FROM_JFIELDID(env, varName, object, varName##_FieldID, type, jniMethod); }

#define GET_STATIC_FIELD_GENERIC(env, varName, clazz, fieldName, sig, type, jniMethod) \
type varName; { \
jfieldID GET_STATIC_JFIELDID(env, varName##_FieldID, clazz, fieldName, sig); \
varName = GET_JFIELD_FROM_JFIELDID(env, varName, clazz, varName##_FieldID, type, jniMethod); }

// Get Boolean Field

#define GET_JBOOLEAN_FIELD(env, varName, object, clazz, fieldName, sig) \
GET_FIELD_GENERIC(env, varName, object, clazz, fieldName, sig, jboolean, GetBooleanField)

#define GET_STATIC_JBOOLEAN_FIELD(env, varName, clazz, fieldName, sig) \
GET_STATIC_FIELD_GENERIC(env, varName, object, clazz, fieldName, sig, jboolean, GetStaticBooleanField)

// Get Byte Field

#define GET_JBYTE_FIELD(env, varName, object, clazz, fieldName, sig) \
GET_FIELD_GENERIC(env, varName, object, clazz, fieldName, sig, jbyte, GetByteField)

#define GET_STATIC_JBYTE_FIELD(env, varName, clazz, fieldName, sig) \
GET_STATIC_FIELD_GENERIC(env, varName, object, clazz, fieldName, sig, jbyte, GetStaticByteField)

// Get Char Field

#define GET_JCHAR_FIELD(env, varName, object, clazz, fieldName, sig) \
GET_FIELD_GENERIC(env, varName, object, clazz, fieldName, sig, jchar, GetCharField)

#define GET_STATIC_JCHAR_FIELD(env, varName, clazz, fieldName, sig) \
GET_STATIC_FIELD_GENERIC(env, varName, object, clazz, fieldName, sig, jchar, GetStaticCharField)

// Get Double Field

#define GET_JDOUBLE_FIELD(env, varName, object, clazz, fieldName, sig) \
GET_FIELD_GENERIC(env, varName, object, clazz, fieldName, sig, jdouble, GetDoubleField)

#define GET_STATIC_JDOUBLE_FIELD(env, varName, clazz, fieldName, sig) \
GET_STATIC_FIELD_GENERIC(env, varName, object, clazz, fieldName, sig, jdouble, GetStaticDoubleField)

// Get Float Field

#define GET_JFLOAT_FIELD(env, varName, object, clazz, fieldName, sig) \
GET_FIELD_GENERIC(env, varName, object, clazz, fieldName, sig, jfloat, GetFloatField)

#define GET_STATIC_JFLOAT_FIELD(env, varName, clazz, fieldName, sig) \
GET_STATIC_FIELD_GENERIC(env, varName, object, clazz, fieldName, sig, jfloat, GetStaticFloatField)

// Get Int Field

#define GET_JINT_FIELD(env, varName, object, clazz, fieldName, sig) \
GET_FIELD_GENERIC(env, varName, object, clazz, fieldName, sig, jint, GetIntField)

#define GET_STATIC_JINT_FIELD(env, varName, clazz, fieldName, sig) \
GET_STATIC_FIELD_GENERIC(env, varName, object, clazz, fieldName, sig, jint, GetStaticIntField)

// Get Long Field

#define GET_JLONG_FIELD(env, varName, object, clazz, fieldName, sig) \
GET_FIELD_GENERIC(env, varName, object, clazz, fieldName, sig, jlong, GetLongField)

#define GET_STATIC_JLONG_FIELD(env, varName, clazz, fieldName, sig) \
GET_STATIC_FIELD_GENERIC(env, varName, object, clazz, fieldName, sig, jlong, GetStaticLongField)

// Get Object Field

#define GET_JOBJECT_FIELD(env, varName, object, clazz, fieldName, sig) \
GET_FIELD_GENERIC(env, varName, object, clazz, fieldName, sig, jobject, GetObjectField)

#define GET_STATIC_JOBJECT_FIELD(env, varName, clazz, fieldName, sig) \
GET_STATIC_FIELD_GENERIC(env, varName, clazz, fieldName, sig, jobject, GetStaticObjectField)

// Get Short Field

#define GET_JSHORT_FIELD(env, varName, object, clazz, fieldName, sig) \
GET_FIELD_GENERIC(env, varName, object, clazz, fieldName, sig, jshort, GetShortField)

#define GET_STATIC_JSHORT_FIELD(env, varName, clazz, fieldName, sig) \
GET_STATIC_FIELD_GENERIC(env, varName, object, clazz, fieldName, sig, jshort, GetStaticShortField)

// Get String Field

#define GET_JSTRING_FIELD(env, varName, object, clazz, fieldName, sig) \
GET_FIELD_GENERIC(env, varName, object, clazz, fieldName, sig, jstring, GetObjectField)

#define GET_STATIC_JSTRING_FIELD(env, varName, clazz, fieldName, sig) \
GET_STATIC_FIELD_GENERIC(env, varName, object, clazz, fieldName, sig, jstring, GetStaticObjectField)

namespace JNIUtils {
	static JavaVM* Jvm;

	// -- Utils Functions --

	static JNIEnv* GetJNIEnv() {
		JNIEnv* env;

		JavaVMAttachArgs args;
		args.version = JNI_VERSION_1_6;
		args.name = NULL;
		args.group = NULL;

		Jvm->AttachCurrentThread(&env, &args);

		return env;
	}

	static std::string ToString(JNIEnv* env, jstring str) {
		jboolean isCopy = true;
		return std::string(env->GetStringUTFChars(str, &isCopy));
	}

	static jobject GetAppActivity(JNIEnv* env) {
		GET_JCLASS(env, unityPlayerClass, "com/unity3d/player/UnityPlayer");
		GET_STATIC_JOBJECT_FIELD(env, appActivity, unityPlayerClass, "currentActivity", "Landroid/app/Activity;");

		return appActivity;
	}

	static jstring GetPackageName(JNIEnv* env) {
		jobject appActivity = GetAppActivity(env);

		CALL_JSTRING_METHOD(env, packageName, appActivity, "getPackageName", "()Ljava/lang/String;");

		return packageName;
	}

	static jstring GetGameVersion(JNIEnv* env) {
		jstring packageName = GetPackageName(env);
		jobject appActivity = GetAppActivity(env);

		CALL_JOBJECT_METHOD(env, packageManager, appActivity, "getPackageManager", "()Landroid/content/pm/PackageManager;");

		CALL_JOBJECT_METHOD(env, packageInfo, packageManager, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;", packageName, 0);
		GET_JCLASS_FROM_JOBJECT(env, packageInfoClass, packageInfo);
		
		GET_JSTRING_FIELD(env, versionName, packageInfo, packageInfoClass, "versionName", "Ljava/lang/String;");

		return versionName;
	}

	// -- Private Functions --

	static void __attribute__((constructor)) OnDlopen() {
		__android_log_print(ANDROID_LOG_VERBOSE, "jni-utils", "Caching Jvm...");

		JNIEnv* env = Modloader::getJni();
		env->GetJavaVM(&Jvm);

		if (Jvm != nullptr) __android_log_print(ANDROID_LOG_VERBOSE, "jni-utils", "Successfully cached Jvm!");
		else __android_log_print(ANDROID_LOG_ERROR, "jni-utils", "Failed to cache Jvm, Jvm is a nullptr!");
	}
}