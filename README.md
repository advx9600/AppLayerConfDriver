AppLayerConfDriver
==================

Ӧ�ò��������������

## Ӧ��
1,��libs/Ŀ¼�µĶ��� copy����Ծ͵�Ŀ¼<br/>
2,�� com.dafeng.appConfDrive �°Ѷ��� copy��src,��������һ��ҲΪcom.dafeng.appConfDrive(��ΪJNI����)

## Example
���ȿ� listConf.add�е�����,���������ó���Ӧ�Ĺ���,����Ҳ�ж�Ӧ���豸����<br/>
S5PAppConDri conf = new S5PAppConDri();<br/>
conf.open("/dev/pin_app_conf_0");<br/>
conf.getPinVal();or conf.getBlockPinVal();<br/>
conf.close();<br/>