package org.opencv.samples.tutorial2;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.imgproc.Imgproc;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.view.View.OnTouchListener;

public class Tutorial2Activity extends Activity implements CvCameraViewListener2 {
    private static final String    TAG = "OCVSample::Activity";

    private static final int       VIEW_MODE_RGBA     = 0;
    private static final int       VIEW_MODE_GRAY     = 1;
    private static final int       VIEW_MODE_CANNY    = 2;
    private static final int       VIEW_MODE_FEATURES = 5;

    private int                    mViewMode;
    private Mat                    mRgba;
    private Mat                    mIntermediateMat;
    private Mat                    mGray;

    private MenuItem               mItemPreviewRGBA;
    private MenuItem               mItemPreviewGray;
    private MenuItem               mItemPreviewCanny;
    private MenuItem               mItemPreviewFeatures;

    private CameraBridgeViewBase   mOpenCvCameraView;

    private BaseLoaderCallback  mLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS:
                {
                    Log.i(TAG, "OpenCV loaded successfully");

                    // Load native library after(!) OpenCV initialization
                    System.loadLibrary("mixed_sample");

                    mOpenCvCameraView.enableView();
                } break;
                default:
                {
                    super.onManagerConnected(status);
                } break;
            }
        }
    };

    public Tutorial2Activity() {
        Log.i(TAG, "Instantiated new " + this.getClass());
    }

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.i(TAG, "called onCreate");
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        setContentView(R.layout.tutorial2_surface_view);

        mOpenCvCameraView = (CameraBridgeViewBase) findViewById(R.id.tutorial2_activity_surface_view);
        mOpenCvCameraView.setCvCameraViewListener(this);
        mOpenCvCameraView.setOnTouchListener(myTouchListener);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        Log.i(TAG, "called onCreateOptionsMenu");
        mItemPreviewRGBA = menu.add("Preview RGBA");
        mItemPreviewGray = menu.add("Preview GRAY");
        mItemPreviewCanny = menu.add("Canny");
        mItemPreviewFeatures = menu.add("Find features");
        return true;
    }

    @Override
    public void onPause()
    {
        super.onPause();
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }

    @Override
    public void onResume()
    {
        super.onResume();
        OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_3, this, mLoaderCallback);
    }

    public void onDestroy() {
        super.onDestroy();
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }

    public void onCameraViewStarted(int width, int height) {
        mRgba = new Mat(height, width, CvType.CV_8UC4);
        mIntermediateMat = new Mat(height, width, CvType.CV_8UC4);
        mGray = new Mat(height, width, CvType.CV_8UC1);
    }

    public void onCameraViewStopped() {
        mRgba.release();
        mGray.release();
        mIntermediateMat.release();
    }

    public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
        final int viewMode = mViewMode;
        switch (viewMode) {
        case VIEW_MODE_GRAY:
            // input frame has gray scale format
            //Imgproc.cvtColor(inputFrame.gray(), mRgba, Imgproc.COLOR_GRAY2RGBA, 4);
        	//mRgba = inputFrame.rgba();
            //mGray = inputFrame.gray();
            //CaptureFrame(0,mGray.getNativeObjAddr(), mRgba.getNativeObjAddr());
            break;
        case VIEW_MODE_RGBA:
            // input frame has RBGA format
            //mRgba = inputFrame.rgba();
        	//mRgba = inputFrame.rgba();
            //mGray = inputFrame.gray();
            //FindFeatures1(mGray.getNativeObjAddr(), mRgba.getNativeObjAddr());
            break;
        case VIEW_MODE_CANNY:
            // input frame has gray scale format
            //mRgba = inputFrame.rgba();
            //Imgproc.Canny(inputFrame.gray(), mIntermediateMat, 80, 100);
            //Imgproc.cvtColor(mIntermediateMat, mRgba, Imgproc.COLOR_GRAY2RGBA, 4);
        	//mRgba = inputFrame.rgba();
            //mGray = inputFrame.gray();
            //FindFeatures2(mGray.getNativeObjAddr(), mRgba.getNativeObjAddr());
            break;
        case VIEW_MODE_FEATURES:
            // input frame has RGBA format
            //mRgba = inputFrame.rgba();
            //mGray = inputFrame.gray();
        	
            break;
        }
        mRgba = inputFrame.rgba();
        mGray = inputFrame.gray();
        FindFeatures1(mGray.getNativeObjAddr(), mRgba.getNativeObjAddr());
        return mRgba;
    }

    public boolean onOptionsItemSelected(MenuItem item) {
        Log.i(TAG, "called onOptionsItemSelected; selected item: " + item);

        if (item == mItemPreviewRGBA) {
            mViewMode = VIEW_MODE_RGBA;
        } else if (item == mItemPreviewGray) {
            mViewMode = VIEW_MODE_GRAY;
        } else if (item == mItemPreviewCanny) {
            mViewMode = VIEW_MODE_CANNY;
        } else if (item == mItemPreviewFeatures) {
            mViewMode = VIEW_MODE_FEATURES;
        }

        return true;
    }
    
 // helper class for getting touch events
    private OnTouchListener myTouchListener = new OnTouchListener() {
    	public boolean onTouch(View v, MotionEvent e){
    		if (mViewMode == VIEW_MODE_GRAY){
    			// we want to capture frames
    			switch (e.getAction()){
    			case MotionEvent.ACTION_UP:
    				//Tap has finished
    				Log.i(TAG, "mylistener called, end of touch");
    				CaptureFrame(0);
    			}
    		}
    		if (mViewMode == VIEW_MODE_RGBA){
    			// we want to capture frames
    			switch (e.getAction()){
    			case MotionEvent.ACTION_UP:
    				//Tap has finished
    				Log.i(TAG, "mylistener called, end of touch");
    				CaptureFrame1(0);
    			}
    		}
    		return true;
    	}
    };
    
    //public native void FindFeatures2(long matAddrGr, long matAddrRgba);
    public native void FindFeatures1(long matAddrGr, long matAddrRgba);
    public native void CaptureFrame(int capture_idx);
    public native void CaptureFrame1(int capture_idx);
}
