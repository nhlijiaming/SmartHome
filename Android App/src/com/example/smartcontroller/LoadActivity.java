package com.example.smartcontroller;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.Animation.AnimationListener;
import android.widget.ImageView;

	public class LoadActivity extends Activity {

		@Override
		protected void onCreate(Bundle savedInstanceState) {
			super.onCreate(savedInstanceState);
			 requestWindowFeature(Window.FEATURE_NO_TITLE); 
			 getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,    
				        WindowManager.LayoutParams.FLAG_FULLSCREEN); 
			setContentView(R.layout.activity_load);
			ImageView loadimage = (ImageView)findViewById(R.id.loadImage);
			AlphaAnimation animation = new AlphaAnimation(0.1f,1.0f);
			animation.setDuration(1000);
			loadimage.setAnimation(animation);
			animation.setAnimationListener(new AnimationListener(){

				@Override
				public void onAnimationEnd(Animation animation) {
					Intent intent = new Intent();
			        intent.setClass(LoadActivity.this, MainActivity.class);
			        startActivity(intent);
			        finish();
			          
				}

				@Override
				public void onAnimationRepeat(Animation animation) {
					// TODO Auto-generated method stub
					
				}

				@Override
				public void onAnimationStart(Animation animation) {
					// TODO Auto-generated method stub
					
				}



			
				});
			}
		}