package com.dafeng.appConfDriver;

public class PinConf {
	static enum SetType {
		INPUT, OUTPUT, IRQ
	}

	static enum IRQType {
		FALLING, RISING, LOW
	}

	private String devName;
	private int pin;
	private SetType type;

	private IRQType irqType;
	private int irqMinTime;

	public IRQType getIrqType() {
		return irqType;
	}

	public void setIrqType(IRQType irqType) {
		this.irqType = irqType;
	}

	public int getIrqMinTime() {
		return irqMinTime;
	}

	public void setIrqMinTime(int irqMinTime) {
		this.irqMinTime = irqMinTime;
	}

	public PinConf(String dev, int pin, SetType type) {
		this(dev, pin, type, null, 0);
	}

	public PinConf(String dev, int pin, IRQType irqType, int irqMinTime) {
		this(dev, pin, SetType.IRQ, irqType, irqMinTime);
	}

	public PinConf(String dev, int pin, SetType type, IRQType irqType,
			int irqMinTime) {
		setDevName(dev);
		setPin(pin);
		setType(type);
		if (type == SetType.IRQ) {
			setIrqType(irqType);
			setIrqMinTime(irqMinTime);
		}
	}

	public String getDevName() {
		return devName;
	}

	public void setDevName(String devName) {
		this.devName = devName;
	}

	public SetType getType() {
		return type;
	}

	public void setType(SetType type) {
		this.type = type;
	}

	public int getPin() {
		return pin;
	}

	public void setPin(int pin) {
		this.pin = pin;
	}
}
