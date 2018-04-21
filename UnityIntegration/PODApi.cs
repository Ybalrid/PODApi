using UnityEngine;
using UnityEngine.XR;
using System.Runtime.InteropServices;

public class PODApi : MonoBehaviour {

    //You will see a lot of private static extern methods imported from the plugin DLL.
    //this basically correspond to the ones in PODApi.h
    [DllImport("PODApi")]
    private static extern void POD_init();

    [DllImport("PODApi")]
    private static extern void POD_exit();

    [DllImport("PODApi")]
    private static extern void POD_update();

    //The plugin implements it's own version of "printf" that can be hooked externally. 
    //We are giving this delegate to the plugin, that is converted to a function pointer
    //taking a "char*" as input in C
    private delegate void unity_debug_callback(string message);

    //The delegate itself is the type of a funciton pointer in C, and here's the funciton
    //that the DLL exposes to get your function
    [DllImport("PODApi")]
    private static extern void register_debug_callback(unity_debug_callback callback);

    //This is the actual method that will print the string into the log and that will be
    //passed via register_debug_callback to the DLL

    //This is called from the DLL, we need the MonoPInvokeCallbakc attribute
    [MonoPInvokeCallback(typeof(unity_debug_callback))]
    private static void native_plugin_console_log(string message)
    {
        Debug.Log("PODApi: " + message);
    }

    //This write the 3 composant of the user's walk speed vector into an array
    [DllImport("PODApi")]
    private static extern void POD_get_walk_linear_speed_vector(float[] array);

    //This return the most recent POD timecode received on the network.
    [DllImport("PODApi")]
    private static extern long  POD_get_most_recent_time_code();

    //Some static data storage
    private static float[] tempArray = {0,0,0};
    private static long mostRecentTimeCode = 0;

    //For unity specific calibration
    private static Vector3 walkSpeedVector = new Vector3();
    private static Quaternion calibration = new Quaternion();
    private static Vector3 viewDirection = new Vector3();
    private static Vector3 forwardVect = new Vector3(0,0,1);
    private static Vector3 upVect = new Vector3(0, 1, 0);

    // Use this for initialization
    void Start ()
    {
        //Initialization of the DLL here
        POD_init();
        register_debug_callback(native_plugin_console_log);

        calibration = Quaternion.identity;

    }

    void OnDestroy()
    {
        //This will do the cleanup
        POD_exit();
    }
	
	// Update is called once per frame
	void Update ()
    {
        viewDirection = GetComponentInChildren<Camera>().transform.rotation * forwardVect;
        //This reads what has been received from the network 
        POD_update();

        //Get the data
        mostRecentTimeCode = POD_get_most_recent_time_code();
        POD_get_walk_linear_speed_vector(tempArray);
        walkSpeedVector.x = tempArray[0];
        walkSpeedVector.y = tempArray[1];
        walkSpeedVector.z = -tempArray[2];
    }

    public static Vector3 getWalkSpeedVector()
    {
        return calibration * walkSpeedVector;
    }

    public static long getMostRecentTimeCode()
    {
        return mostRecentTimeCode;
    }

    public static void calibrate()
    {
        viewDirection.y = 0;
        viewDirection.Normalize();

        Debug.Log("View direction is " + viewDirection);

        float angle = Vector3.Angle(viewDirection, forwardVect);

        Debug.Log("Calculated angle is " + angle);

        calibration = Quaternion.AngleAxis(-angle, upVect);

    }
}
