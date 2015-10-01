package com.example.smartcontroller;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;
import java.util.UUID;

import android.app.Activity;
import android.app.PendingIntent;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.ActivityNotFoundException;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.speech.RecognizerIntent;
import android.telephony.SmsManager;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {
	private static final String TAG = "Smart Controller";

	private static final boolean D = true;

	private BluetoothAdapter mBluetoothAdapter = null;

	private BluetoothSocket btSocket = null;

	private OutputStream outStream = null;
	private InputStream inStream = null;

	private final BroadcastReceiver mBroadcastReceiver = new MyBroadcastReceiver ();
	SmsManager sManager;
	Button button, buttonConnect;
	ImageButton imageButton;
	Switch switcher1;
	Switch switcher2;
	Switch switcher3;
	Switch switcher4;
	EditText editText; 
	Handler handler;
	private EditText et_phone;
    private Button bto1;
    private Button btf1;
    private Button bto2;
    private Button btf2;
    private Button bto3;
    private Button btf3;
    private Button bto4;
    private Button btf4;
	private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
	private static String address = "20:13:11:07:24:55";

	protected static final int RESULT_SPEECH = 1;

	/** Called when the activity is first created. */

 
	public void onCreate(Bundle savedInstanceState) {

		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		editText = (EditText)findViewById(R.id.editText1);
		editText.setFocusable(false);
		handler = new MyHandler();
		
		buttonConnect=(Button)findViewById(R.id.btnConnect);
		buttonConnect.setOnClickListener(new Button.OnClickListener(){
 
			public void onClick(View v) {
				BT_Connect();
			}
		});

		imageButton=(ImageButton)findViewById(R.id.btnVoice);
		imageButton.setOnClickListener(new Button.OnClickListener(){
 
			public void onClick(View v) {
				StartSpeak();
			}
		}); 
		switcher1=(Switch)findViewById(R.id.switcher1);
		switcher1.setOnClickListener(new OnClickListener(){
 
			public void onClick(View arg0) { 
				boolean ret;
				if (switcher1.isChecked()) {
					ret = BT_Send("$L1On@");
					if (ret) {

					}
					else {
						switcher1.setChecked(!switcher1.isChecked()); 
					}
				}
				else
				{
					ret = BT_Send("$L1Off@");
					if (ret) {

					}
					else {
						switcher1.setChecked(!switcher1.isChecked()); 
					}
				}
			}

		});

		switcher2=(Switch)findViewById(R.id.switcher2);
		switcher2.setOnClickListener(new OnClickListener(){
 
			public void onClick(View arg0) { 
				boolean ret;
				if (switcher2.isChecked()) {
					ret = BT_Send("$L2On@");
					if (ret) {

					}
					else {
						switcher2.setChecked(!switcher2.isChecked()); 					}
				}
				else
				{
					ret = BT_Send("$L2Off@");
					if (ret) {

					}
					else {
						switcher2.setChecked(!switcher2.isChecked()); 					}
				}
			}

		});

		switcher3=(Switch)findViewById(R.id.switcher3);
		switcher3.setOnClickListener(new OnClickListener(){
 
			public void onClick(View arg0) { 
				boolean ret;
				if (switcher3.isChecked()) {
					ret = BT_Send("$L3On@");
					if (ret) {

					}
					else {
						switcher3.setChecked(!switcher3.isChecked()); 					}
				}
				else
				{
					ret = BT_Send("$L3Off@");
					if (ret) {

					}
					else {
						switcher3.setChecked(!switcher3.isChecked()); 					}
				}
			}

		});

		switcher4=(Switch)findViewById(R.id.switcher4);
		switcher4.setOnClickListener(new OnClickListener(){
 
			public void onClick(View arg0) { 
				boolean ret;
				if (switcher4.isChecked()) {
					ret = BT_Send("$L4On@");
					if (ret) {

					}
					else {
						switcher4.setChecked(!switcher4.isChecked()); 					}
				}
				else
				{
					ret = BT_Send("$L4Off@");
					if (ret) {

					}
					else {
						switcher4.setChecked(!switcher4.isChecked()); 					}
				}
			}

		});
		
	       et_phone = (EditText) findViewById(R.id.et_phone);
	        bto1 = (Button)findViewById(R.id.bto1);	        
	        bto1.setOnClickListener(new OnClickListener()
	        {
	            
	            @Override
	            public void onClick(View v)
	            {
	                String mobile = et_phone.getText().toString();
	                String content = "$L1On@";
	                
	                sManager = SmsManager.getDefault();
	                
	                PendingIntent sentIntent = PendingIntent.getBroadcast(MainActivity.this, 0, new Intent(), 0);

	                if(mobile != null){
	                	try{
	                    sManager.sendTextMessage(mobile, null, content, sentIntent, null);
	                    DisplayToast("发送成功！！");
	                	}catch(Exception e){
	                	DisplayToast("亲~请先输入手机号！");
	                	}
	                   
	                } 
	                
	            }
	        });   
	        btf1 = (Button)findViewById(R.id.btf1);	        
	        btf1.setOnClickListener(new OnClickListener()
	        {
	            
	            @Override
	            public void onClick(View v)
	            {
	                String mobile = et_phone.getText().toString();
	                String content = "$L1Off@";
	                
	                sManager = SmsManager.getDefault();
	                
	                PendingIntent sentIntent = PendingIntent.getBroadcast(MainActivity.this, 0, new Intent(), 0);

	                if(mobile != null){
	                	try{
	                    sManager.sendTextMessage(mobile, null, content, sentIntent, null);
	                    DisplayToast("发送成功！！");
	                	}catch(Exception e){
	                	DisplayToast("亲~请先输入手机号！");
	                	}
	                   
	                } 
	                
	            }
	        }); 
	        bto2 = (Button)findViewById(R.id.bto2);	        
	        bto2.setOnClickListener(new OnClickListener()
	        {
	            
	            @Override
	            public void onClick(View v)
	            {
	                String mobile = et_phone.getText().toString();
	                String content = "$L2On@";
	                
	                sManager = SmsManager.getDefault();
	                
	                PendingIntent sentIntent = PendingIntent.getBroadcast(MainActivity.this, 0, new Intent(), 0);

	                if(mobile != null){
	                	try{
	                    sManager.sendTextMessage(mobile, null, content, sentIntent, null);
	                    DisplayToast("发送成功！！");
	                	}catch(Exception e){
	                	DisplayToast("亲~请先输入手机号！");
	                	}
	                   
	                } 
	                
	            }
	        });     
	        btf2 = (Button)findViewById(R.id.btf2);	        
	        btf2.setOnClickListener(new OnClickListener()
	        {
	            
	            @Override
	            public void onClick(View v)
	            {
	                String mobile = et_phone.getText().toString();
	                String content = "$L2Off@";
	                
	                sManager = SmsManager.getDefault();
	                
	                PendingIntent sentIntent = PendingIntent.getBroadcast(MainActivity.this, 0, new Intent(), 0);

	                if(mobile != null){
	                	try{
	                    sManager.sendTextMessage(mobile, null, content, sentIntent, null);
	                    DisplayToast("发送成功！！");
	                	}catch(Exception e){
	                	DisplayToast("亲~请先输入手机号！");
	                	}
	                   
	                } 
	                
	            }
	        });
	        bto3 = (Button)findViewById(R.id.bto3);	        
	        bto3.setOnClickListener(new OnClickListener()
	        {
	            
	            @Override
	            public void onClick(View v)
	            {
	                String mobile = et_phone.getText().toString();
	                String content = "$L3On@";
	                
	                sManager = SmsManager.getDefault();
	                
	                PendingIntent sentIntent = PendingIntent.getBroadcast(MainActivity.this, 0, new Intent(), 0);

	                if(mobile != null){
	                	try{
	                    sManager.sendTextMessage(mobile, null, content, sentIntent, null);
	                    DisplayToast("发送成功！！");
	                	}catch(Exception e){
	                	DisplayToast("亲~请先输入手机号！");
	                	}
	                   
	                } 
	                
	            }
	        });
	        btf3 = (Button)findViewById(R.id.btf3);	        
	        btf3.setOnClickListener(new OnClickListener()
	        {
	            
	            @Override
	            public void onClick(View v)
	            {
	                String mobile = et_phone.getText().toString();
	                String content = "$L3Off@";
	                
	                sManager = SmsManager.getDefault();
	                
	                PendingIntent sentIntent = PendingIntent.getBroadcast(MainActivity.this, 0, new Intent(), 0);

	                if(mobile != null){
	                	try{
	                    sManager.sendTextMessage(mobile, null, content, sentIntent, null);
	                    DisplayToast("发送成功！！");
	                	}catch(Exception e){
	                	DisplayToast("亲~请先输入手机号！");
	                	}
	                   
	                } 
	                
	            }
	        });
	        bto4 = (Button)findViewById(R.id.bto4);	        
	        bto4.setOnClickListener(new OnClickListener()
	        {
	            
	            @Override
	            public void onClick(View v)
	            {
	                String mobile = et_phone.getText().toString();
	                String content = "$L4On@";
	                
	                sManager = SmsManager.getDefault();
	                
	                PendingIntent sentIntent = PendingIntent.getBroadcast(MainActivity.this, 0, new Intent(), 0);

	                if(mobile != null){
	                	try{
	                    sManager.sendTextMessage(mobile, null, content, sentIntent, null);
	                    DisplayToast("发送成功！！");
	                	}catch(Exception e){
	                	DisplayToast("亲~请先输入手机号！");
	                	}
	                   
	                } 
	                
	            }
	        });
	        btf4 = (Button)findViewById(R.id.btf4);	        
	        btf4.setOnClickListener(new OnClickListener()
	        {
	            
	            @Override
	            public void onClick(View v)
	            {
	                String mobile = et_phone.getText().toString();
	                String content = "$L4Off@";
	                
	                sManager = SmsManager.getDefault();
	                
	                PendingIntent sentIntent = PendingIntent.getBroadcast(MainActivity.this, 0, new Intent(), 0);
	                if(mobile != null){
	                	try{
	                    sManager.sendTextMessage(mobile, null, content, sentIntent, null);
	                    DisplayToast("发送成功！！");
	                	}catch(Exception e){
	                	DisplayToast("亲~请先输入手机号！");
	                	}
	                   
	                } 
	                
	            }
	        });     
		if (D)
			Log.e(TAG, "+++ ON CREATE +++");
	}
 

	public void onStart() {

		super.onStart();
		if (D) Log.e(TAG, "++ ON START ++");
		
		BT_Connect();
		
	}
 

	public void onResume() {

		super.onResume();
		if (D) Log.e(TAG, "+ ON Resume +");
	}
 
	public void onPause() {

		super.onPause();

		if (D)
			Log.e(TAG, "- ON PAUSE -");

	} 
	public void onStop() {

		super.onStop();

		if (D)Log.e(TAG, "-- ON STOP --");

		try {
			unregisterReceiver(mBroadcastReceiver);
		}
		catch (IllegalArgumentException e){
			
		}
		if (timer != null) {
			timer.cancel();
			timer = null;
		}


		if (outStream != null) {
			try {
				outStream.flush();
			} catch (IOException e) {
				Log.e(TAG, "ON PAUSE: Couldn't flush output stream.", e);
			}

		}

		if (btSocket != null)
		{
			try {
				btSocket.close();
				DisplayToast("蓝牙连接已断开");
			} catch (IOException e2) {

				DisplayToast("套接字关闭失败！");
			}
		}
		

	} 

	public void onDestroy() {

		super.onDestroy();

		if (D) Log.e(TAG, "--- ON DESTROY ---");

	} 
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);

		switch (requestCode) {
		case RESULT_SPEECH: {
			if (resultCode == RESULT_OK && null != data) {

				ArrayList<String> text = data
						.getStringArrayListExtra(RecognizerIntent.EXTRA_RESULTS);

				editText.setText(text.get(0));
				RecognizeEngine(editText.getText().toString());
			}
			break;
		}

		}
	}

	public class MyBroadcastReceiver extends BroadcastReceiver { 
		public void onReceive(Context context, Intent intent) { 
			if (intent.getAction().equals("android.bluetooth.device.action.ACL_DISCONNECTED")) {
				ConnectionBroken();
			}
				
		}
	}
	
	class MyHandler extends Handler { 
		public void handleMessage(Message msg) {
			TextView textView;
			int GI;
			Log.e("test", "handler");
			String result = msg.getData().getString("message");
			Log.e(TAG, "res :"+result);
			if (result.length()>3) {
				Log.e(TAG, "1");
				if (result.charAt(0)=='$') {
					Log.e(TAG, "2:"+result.substring(1, 3)+";");
					
					if (result.substring(1, 3).equals("TH")) {
						textView = (TextView)findViewById(R.id.textView10);
						textView.setText("" + result.substring(3, 5) + "℃");
						textView = (TextView)findViewById(R.id.textView11);
						textView.setText("" + result.substring(6, 8) + "%");
					}
					
					if (result.substring(1, 3).equals("LI")) {
						textView = (TextView)findViewById(R.id.textView12);
						textView.setText("" + result.substring(3, result.length()-1) + " lx");
					}
					
					if (result.substring(1, 3).equals("GI")) {
						textView = (TextView)findViewById(R.id.textView13);
						GI = Integer.parseInt(result.substring(3, result.length()-1));
						
						textView.setText("" + result.substring(3, result.length()-1) + "%");
						if (GI<15) textView.setTextColor(getResources().getColor(R.color.blue)); 
						else textView.setTextColor(getResources().getColor(R.color.red));
					}
					
					if (result.substring(1, 3).equals("SS")) {
						
						textView = (TextView)findViewById(R.id.textView14);
						if (result.substring(3, 4).equals("1")) {
							textView.setText("开");
							textView.setTextColor(getResources().getColor(R.color.red));
						}
						else{
							textView.setText("关");
							textView.setTextColor(getResources().getColor(R.color.blue));
						}
						
						textView = (TextView)findViewById(R.id.textView15);
						
						if (result.substring(4, 5).equals("1")) {
							textView.setText("开");
							textView.setTextColor(getResources().getColor(R.color.red));
						}
						else{
							textView.setText("关");
							textView.setTextColor(getResources().getColor(R.color.blue));
						}
					}

				}

			}
		}
	}

	public void DisplayToast(String str)
	{
		Toast toast=Toast.makeText(this, str, Toast.LENGTH_LONG);
		toast.show();
	}

	public void BT_Connect() {

		TextView textViewStatus = (TextView)findViewById(R.id.textView1);

		mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

		if (mBluetoothAdapter == null) { 
			textViewStatus.setText("蓝牙设备不可用，请打开蓝牙！");
			textViewStatus.setTextColor(getResources().getColor(R.color.red));
			return;
		}

		if (!mBluetoothAdapter.isEnabled()) {
			Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
			startActivity(intent);
		}

		textViewStatus.setText("正在连接....");
		textViewStatus.setTextColor(getResources().getColor(R.color.black));
		BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
		try {

			btSocket = device.createRfcommSocketToServiceRecord(MY_UUID);

		} catch (IOException e) {

			DisplayToast("套接字创建失败！");
			textViewStatus.setText("套接字创建失败！");
			textViewStatus.setTextColor(getResources().getColor(R.color.red));
		}
		mBluetoothAdapter.cancelDiscovery();

		try {

			btSocket.connect();
			DisplayToast("已连接智能控制器");
			textViewStatus.setText("已连接");
			IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_ACL_DISCONNECTED);
			registerReceiver(mBroadcastReceiver, filter); 
			setTimer();
			imageButton.setVisibility(View.VISIBLE);
			buttonConnect.setVisibility(View.INVISIBLE);
			StartSpeak();
			BT_Send("$GetTH@");
			BT_Send("$GetLI@");
			BT_Send("$GetGI@");
			BT_Send("$GetSen@");

		} catch (IOException e) {
			try {
				btSocket.close();

			} catch (IOException e2) {

				DisplayToast("连接没有建立，无法关闭套接字！");
				textViewStatus.setText("连接没有建立，无法关闭套接字！");
			}
			DisplayToast("连接失败");
			textViewStatus.setText("连接失败");
			textViewStatus.setTextColor(getResources().getColor(R.color.red));
		}
	}

	public boolean BT_Send(String message)
	{
		byte[] msgBuffer;
		if (btSocket == null) {
			DisplayToast("请先建立连接");
			return false;
		}
		try {
			outStream = btSocket.getOutputStream();
		} catch (IOException e) {
			Log.e(TAG, "ON RESUME: Output stream creation failed.", e);
			return false;
		}

		msgBuffer = message.getBytes();

		try {

			outStream.write(msgBuffer);

		} catch (IOException e) {

			Log.e(TAG, "ON RESUME: Exception during write.", e);
			ConnectionBroken();
			return false;

		}
		return true;
	}


	Timer timer;
	TimerTask task;
	public void setTimer() {
		task = new TimerTask(){
			int msgBuff, msgBuffc, rec;
			String str;	
			
			public void run() {
				msgBuffc = 0;
				try {
					inStream = btSocket.getInputStream();
					msgBuffc = inStream.available();
				} catch (IOException e) {
					Log.e(TAG, "ON RESUME: Input stream creation failed.", e);
				}
				str = "";
				rec = 0;
				for(; msgBuffc > 0 ; msgBuffc --)
				{
					try { 

						msgBuff = inStream.read(); 

					} catch (IOException e) { 
						Log.e(TAG, "IO exc");
						continue;
					} 
					if (msgBuff == (int)'$') rec = 1;
					if (rec == 1) str = str + (char)msgBuff;
					if (msgBuff == (int)'@') {
						rec = 0;
						Log.e(TAG, "message:"+str);
						Message result = handler.obtainMessage();
						Bundle data = new Bundle();
						data.putString("message", str);
						result.setData(data);
						handler.sendMessage(result);
						str = "";
					}
				}

			}
		};
		timer = new Timer();
		timer.schedule(task, 100, 1000); 
	}

	private void RecognizeEngine(String str) {

		boolean ret;
		if (str.indexOf("开") > -1) {

			if (str.indexOf("灯") > -1) {
				ret = BT_Send("$L1On@");
				if (ret) {
					switcher1.setChecked(true);
				}
			}

			if (str.indexOf("风扇") > -1) {
				ret = BT_Send("$L2On@");
				if (ret) {
					switcher2.setChecked(true);
				}
			}		
			if (str.indexOf("空调") > -1) {
				ret = BT_Send("$L3On@");
				if (ret) {
					switcher3.setChecked(true);
				}
			}		
			if (str.indexOf("音响") > -1) {
				ret = BT_Send("$L4On@");
				if (ret) {
					switcher4.setChecked(true);
				}
			}		
		}

		if (str.indexOf("关") > -1) {

			if (str.indexOf("灯") > -1) {
				ret = BT_Send("$L1Off@");
				if (ret) {
					switcher1.setChecked(false);
				}
			}

			if (str.indexOf("风扇") > -1) {
				ret = BT_Send("$L2Off@");
				if (ret) {
					switcher2.setChecked(false);
				}
			}		
			if (str.indexOf("空调") > -1) {
				ret = BT_Send("$L3Off@");
				if (ret) {
					switcher3.setChecked(false);
				}
			}		
			if (str.indexOf("音响") > -1) {
				ret = BT_Send("$L4Off@");
				if (ret) {
					switcher4.setChecked(false);
				}
			}		
		}
	}

	public void StartSpeak() {

		Intent intent = new Intent(
				RecognizerIntent.ACTION_RECOGNIZE_SPEECH);

		intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE_MODEL, RecognizerIntent.LANGUAGE_MODEL_FREE_FORM);  
		intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE, "zh");
		intent.putExtra(RecognizerIntent.EXTRA_PROMPT, "请说指令");

		try {
			startActivityForResult(intent, RESULT_SPEECH);
			editText.setText("");
		} catch (ActivityNotFoundException a) {
			Toast t = Toast.makeText(getApplicationContext(),
					"Ops! Your device doesn't support Speech to Text",
					Toast.LENGTH_SHORT);
			t.show();
		}
	}
	
	private void ConnectionBroken() {
		btSocket = null;
	}

}