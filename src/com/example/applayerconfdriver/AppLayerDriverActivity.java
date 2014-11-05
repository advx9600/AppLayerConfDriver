package com.example.applayerconfdriver;

import com.dafeng.appConfDriver.S5PAppConDri;

import android.support.v7.app.ActionBarActivity;
import android.support.v4.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

public class AppLayerDriverActivity extends ActionBarActivity {

	private S5PAppConDri mAppCon;
	private boolean mIsExist = false;

	class ThreadReadBlock extends Thread {
		@Override
		public void run() {
			while (!mIsExist) {
				mAppCon.getBlockPinVal();
				a.b("getBlockPinVal");
			}
		}
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.fragment_app_layer_driver);
		//
		// if (savedInstanceState == null) {
		// getSupportFragmentManager().beginTransaction()
		// .add(R.id.container, new PlaceholderFragment()).commit();
		// }

		mAppCon = new S5PAppConDri();
		mAppCon.open("/dev/pin_app_conf_0");
		new ThreadReadBlock().start();
		// apCon.setConfFirst(93, 0);
	}

	public void onDestroy() {
		super.onDestroy();
		mAppCon.close();
		mIsExist = true;
	}

	public void test1(View v) {
		mAppCon.setPinVal(1);
	}

	public void test2(View v) {
		mAppCon.setPinVal(0);
	}

	public void test3(View v) {
		a.c("" + mAppCon.getPinVal());
		toast(mAppCon.getPinVal() + "");
	}

	private void toast(String msg) {
		Toast.makeText(this, msg, Toast.LENGTH_LONG).show();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {

		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.app_layer_driver, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

	/**
	 * A placeholder fragment containing a simple view.
	 */
	public static class PlaceholderFragment extends Fragment {

		public PlaceholderFragment() {
		}

		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container,
				Bundle savedInstanceState) {
			View rootView = inflater.inflate(
					R.layout.fragment_app_layer_driver, container, false);
			return rootView;
		}
	}

}
