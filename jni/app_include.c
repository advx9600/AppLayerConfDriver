static int is_used_pin(const char* dev) {
	int isUsed = 0;
	int fd = open(dev, O_RDONLY);
	if (fd < 0) {
		perror("open");
		return -1;
	}
	Write_a wa;
	wa.ops = APP_OPS_READ_IS_USED;
	read(fd, &wa, sizeof(wa));
	if (wa.isUsed) {
		isUsed = 1;
	}
	close(fd);

	return isUsed;
}

static int set_app_set_type(Write_a* a) {
	int isFailed = 0;
	int fd = open(a->devName, O_RDWR);
	if (fd < 0) {
		perror("open");
		return -1;
	}
	if (write(fd, a, sizeof(Write_a)) < 0) {
		printf("set irq failed!\n");
		isFailed = 1;
	}
	close(fd);
	return isFailed;
}
static int set_app_irq(Write_a* a) {
	return set_app_set_type(a);
}

static int set_app_output(Write_a* a) {
	return set_app_set_type(a);
}

static int set_app_input(Write_a* a) {
	return set_app_set_type(a);
}

static int irq_test() {
	const char* dev = "/dev/pin_app_conf_0";
	int i;
	if (is_used_pin(dev)) {
		printf("alread used pin!\n");
		return -1;
	}
	Write_a a;
	strcpy(a.devName, dev);
	a.ops = APP_OPS_SET_IRQ;
	a.irqType = APP_OPS_IRQ_FALLING;
	a.irqMinTime = 500;
	strcpy(a.pinName, "S5PV210_GPH0");
	a.pinNum = 6;
	if (set_app_irq(&a)) {
		printf("set_app_irq failed!\n");
		return -1;
	}
	int fd = open(dev, O_RDWR);
	Write_a b;
	for (i = 0; i < 30; i++) {
		b.ops = APP_OPS_READ_VAL;
		b.isBlockRead = 0; // 1 or 0
		sleep(1);
		if (read(fd, &b, sizeof(b)) > 0)
			printf("%d,val:%d\n", i, b.pinVal);
	}
	close(fd);
	return 0;
}

static int output_test() {
	const char* dev = "/dev/pin_app_conf_0";
	int i;
	if (is_used_pin(dev)) {
		printf("alread used pin!\n");
		return -1;
	}

	Write_a a;
	strcpy(a.devName, dev);
	a.ops = APP_OPS_SET_OUTPUT;
	strcpy(a.pinName, "S5PV210_GPH0");
	a.pinNum = 6;

	if (set_app_output(&a)) {
		printf("set_app_output failed!\n");
	}

	int fd = open(dev, O_RDWR);
	Write_a b;
	for (i = 0; i < 30; i++) {
		b.ops = APP_OPS_SET_PIN_VAL;
		b.setPinVal = i % 2;
		if (write(fd, &b, sizeof(b)) < 0) {
			perror("write");
		}
		sleep(1);
		b.ops = APP_OPS_READ_VAL;
		if (read(fd, &b, sizeof(b)) > 0)
			printf("%d,val:%d\n", i, b.pinVal);
	}
	close(fd);

	return 0;
}

static int input_test() {
	const char* dev = "/dev/pin_app_conf_0";
	int i;
	if (is_used_pin(dev)) {
		printf("alread used pin!\n");
		return -1;
	}

	Write_a a;
	strcpy(a.devName, dev);
	a.ops = APP_OPS_SET_INPUT;
	strcpy(a.pinName, "S5PV210_GPH0");
	a.pinNum = 6;

	if (set_app_input(&a)) {
		printf("set_app_input failed!\n");
	}

	int fd = open(dev, O_RDWR);
	Write_a b;
	for (i = 0; i < 30; i++) {
		sleep(2);
		b.ops = APP_OPS_READ_VAL;
		if (read(fd, &b, sizeof(b)) > 0)
			printf("%d,val:%d\n", i, b.pinVal);
	}
	close(fd);

	return 0;
}
