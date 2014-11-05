#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

#include <jni.h>
#include <android/log.h>
#define TAG "app_driver_android.c" // 这个是自定义的LOG的标识
#define LOGI(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__)
#define perror LOGE

#include "pin_app.h"
#include "app_include.c"

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

jboolean Java_com_dafeng_appConfDriver_S5PAppConDri_nativeIsUsedDev(JNIEnv* env,
		jobject thiz, jstring dev) {
	const char *nativeDev = (*env)->GetStringUTFChars(env, dev, 0);
	int ret = is_used_pin(nativeDev);
	(*env)->ReleaseStringUTFChars(env, dev, nativeDev);
	return ret;
}

static void getNameAndNumber(const char* pinName, char* getPinName, int* num) {
	char buf[20];
	strcpy(buf, pinName);
	strcpy(getPinName, "S5PV210_");
	buf[4] = '\0';
	strcat(getPinName, buf);
	*num = (buf[5] - '0' & 0xff);
	LOGI("pinName:%s,pinNum:%d", getPinName, *num);
}

static jboolean set_output_or_input(JNIEnv* env, jstring dev, jstring pinName,
		jint type, jint irqType, jint irqMinTime) {
	const char *nativeDev = (*env)->GetStringUTFChars(env, dev, 0);
	const char *nativePinName = (*env)->GetStringUTFChars(env, pinName, 0);
	int ret = JNI_FALSE;

	Write_a a;
	strcpy(a.devName, nativeDev);
	a.ops = type ? APP_OPS_SET_OUTPUT : APP_OPS_SET_INPUT;
	if (type > 1) {
		a.ops = APP_OPS_SET_IRQ;
		switch (irqType) {
		case 1:
			a.irqType = APP_OPS_IRQ_FALLING;
			break;
		case 2:
			a.irqType = APP_OPS_IRQ_RISING;
			break;
		case 3:
			a.irqType = APP_OPS_IRQ_LOW;
			break;
		}
		a.irqMinTime = irqMinTime;
	}
	getNameAndNumber(nativePinName, a.pinName, &a.pinNum);

	if (set_app_set_type(&a)) {
		ret = JNI_TRUE;
	}

	(*env)->ReleaseStringUTFChars(env, dev, nativeDev);
	(*env)->ReleaseStringUTFChars(env, pinName, nativePinName);
	return ret;
}
jboolean Java_com_dafeng_appConfDriver_S5PAppConDri_nativeSetOutput(JNIEnv* env,
		jobject thiz, jstring dev, jstring pinName) {
	return set_output_or_input(env, dev, pinName, 0, 0, 0);
}

jboolean Java_com_dafeng_appConfDriver_S5PAppConDri_nativeSetInput(JNIEnv* env,
		jobject thiz, jstring dev, jstring pinName) {
	return set_output_or_input(env, dev, pinName, 1, 0, 0);
}

jboolean Java_com_dafeng_appConfDriver_S5PAppConDri_nativeSetIrq(JNIEnv* env,
		jobject thiz, jstring dev, jstring pinName, int irqType, int irqMinTime) {
	return set_output_or_input(env, dev, pinName, 2, irqType, irqMinTime);
}

jint Java_com_dafeng_appConfDriver_S5PAppConDri_nativeOpen(JNIEnv* env,
		jobject thiz, jstring dev) {
	int fd = -1;
	const char *nativeDev = (*env)->GetStringUTFChars(env, dev, 0);
	fd = open(nativeDev, O_RDWR);
	(*env)->ReleaseStringUTFChars(env, dev, nativeDev);
	return fd;
}

void Java_com_dafeng_appConfDriver_S5PAppConDri_nativeClose(JNIEnv* env,
		jobject thiz, jint fd) {
	close(fd);
}

jboolean Java_com_dafeng_appConfDriver_S5PAppConDri_nativeSetPinVal(JNIEnv* env,
		jobject thiz, jint fd, jint val) {
	Write_a b;
	b.ops = APP_OPS_SET_PIN_VAL;
	b.setPinVal = val;
	if (write(fd, &b, sizeof(b)) < 0) {
		perror("write");
		return JNI_FALSE;
	}
	return JNI_TRUE;
}

static jint read_pin_val(jint fd, jint isBlock) {
	Write_a b;
	int ret;
	b.ops = APP_OPS_READ_VAL;
	b.isBlockRead = isBlock ? 1 : 0;
	if ((ret = read(fd, &b, sizeof(b))) < 0) {
		perror("read");
		return -1;
	}
	return b.pinVal;
}

jint Java_com_dafeng_appConfDriver_S5PAppConDri_nativeReadPinVal(JNIEnv* env,
		jobject thiz, jint fd) {
	return read_pin_val(fd, 0);
}

jint Java_com_dafeng_appConfDriver_S5PAppConDri_nativeReadBlockPinVal(
		JNIEnv* env, jobject thiz, jint fd) {
	return read_pin_val(fd, 1);
}
