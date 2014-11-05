package com.dafeng.appConfDriver;

import android.annotation.SuppressLint;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.dafeng.appConfDriver.PinConf.IRQType;
import com.example.applayerconfdriver.a;

public class S5PAppConDri extends BaseAppConDri {
	@SuppressLint("UseSparseArrays")
	private static Map<Integer, String> mMap = new HashMap<Integer, String>();
	private final static String mDefDriverName = "/dev/pin_app_conf_";

	private final static int IRQ_FALLING = 1;
	private final static int IRQ_RISING = 2;
	private final static int IRQ_LOW = 3;

	static {
		for (int i = 0; i < 2; i++)
			mMap.put(92 + i, "GPH0(" + i + ")");
		for (int i = 0; i < 5; i++)
			mMap.put(94 + i, "GPH0(" + (3 + i) + ")");
		for (int i = 0; i < 2; i++)
			mMap.put(99 + i, "GPH1(" + (2 + i) + ")");
		for (int i = 0; i < 8; i++)
			mMap.put(101 + i, "GPH2(" + i + ")");
		System.loadLibrary("AppConDri");

		List<PinConf> listConf = new ArrayList<PinConf>();
		/*
		 * 把97脚配置成中断 ,设备名为 /dev/pin_app_conf_0
		 */
		listConf.add(new PinConf(mDefDriverName + 0, 97, IRQType.FALLING, 1000));
		setConfigs(listConf);
	}

	private static void setConfigs(List<PinConf> list) {
		for (int i = 0; i < list.size(); i++) {
			PinConf conf = list.get(i);
			if (i == 0 && nativeIsUsedDev(conf.getDevName())) {
				a.c("already configed");
				break;
			} else {
				if (setConfig(conf)) {
					a.c("setConfig " + conf.getDevName() + " setType:"
							+ conf.getType() + " success!");

				} else {
					a.e("setConfig " + conf.getDevName() + " setType:"
							+ conf.getType() + " failed!");
				}
			}
		}
	}

	private static boolean setConfig(PinConf conf) {
		String pinNum = mMap.get(conf.getPin());
		boolean ret = false;
		if (pinNum == null || pinNum.length() == 0) {
			a.e("mMap.getPin is NULL");
			return false;
		}
		switch (conf.getType()) {
		case INPUT:
			if (!nativeSetInput(conf.getDevName(), pinNum)) {
				ret = true;
			}
			break;
		case OUTPUT:
			if (!nativeSetOutput(conf.getDevName(), pinNum)) {
				ret = true;
			}
			break;
		case IRQ:
			int irqType = -1;
			switch (conf.getIrqType()) {
			case FALLING:
				irqType = IRQ_FALLING;
				break;
			case RISING:
				irqType = IRQ_RISING;
				break;
			case LOW:
				irqType = IRQ_LOW;
				break;
			default:
				break;
			}
			if (irqType == -1) {
				a.e("not allowed irq type");
				break;
			}
			if (!nativeSetIrq(conf.getDevName(), pinNum, irqType,
					conf.getIrqMinTime())) {
				ret = true;
			}
			break;
		default:
			break;
		}
		return ret;
	}

	private int mFd = -1;

	public boolean open(String dev) {
		if ((mFd = nativeOpen(dev)) > 0) {
			return true;
		}
		return false;
	}

	public void close() {
		nativeClose(mFd);
	}

	public boolean setPinVal(int val) {
		return nativeSetPinVal(mFd, val);
	}

	public int getPinVal() {
		return nativeReadPinVal(mFd);
	}

	public int getBlockPinVal() {
		return nativeReadBlockPinVal(mFd);
	}

	private static native int nativeOpen(String dev);

	private static native boolean nativeSetPinVal(int fd, int val);

	private static native int nativeReadPinVal(int fd);

	private static native int nativeReadBlockPinVal(int fd);

	private static native void nativeClose(int fd);

	private static native boolean nativeIsUsedDev(String dev);

	private static native boolean nativeSetOutput(String devName, String pinName);

	private static native boolean nativeSetInput(String devName, String pinName);

	private static native boolean nativeSetIrq(String devName, String pinName,
			int irqType, int irqMinTime);
}
