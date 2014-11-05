AppLayerConfDriver
==================

应用层对驱动进行配置

## 应用
1,把libs/目录下的东西 copy到相对就的目录<br/>
2,把 com.dafeng.appConfDrive 下把东西 copy到src,包名必须一样也为com.dafeng.appConfDrive(因为JNI调用)

## Example
首先看 listConf.add中的数据,把引脚设置成相应的功能,其中也有对应的设备名称<br/>
S5PAppConDri conf = new S5PAppConDri();<br/>
conf.open("/dev/pin_app_conf_0");<br/>
conf.getPinVal();or conf.getBlockPinVal();<br/>
conf.close();<br/>