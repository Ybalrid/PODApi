using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Sample script that show how to get data from POD in Unity.
/// You need to have POD in the same network as you, to have the
/// DLL and PODApi.cs script in your assets, and to PODApi to ONE 
/// game object of the scene. 
/// </summary>
public class PODApiUser : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update ()
    {
        //This is basically the only thing you have to do:
        Debug.Log(PODApi.getWalkSpeedVector());

        //Set this as the speed of the player's rigid body, or multipy this by
        //the frame time to get a distance vector and move your camera with it.
    }
}
