using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Net.Sockets;
using System.Threading;

public class testDll : MonoBehaviour {
    //variables
    private bool _threadRunning;
    private Thread _thread;
    private TCPConnection myTCP;
    private string serverMsg;
    public string msgToServer;

    public float smooth = 10.0f;

    public GameObject wiiIndicator;

    public double deadzoneXH = 50.00;
    public double deadzoneZH = 50.00;
    public double deadzoneXL = -50.00;
    public double deadzoneZL = -50.00;

    public double limitXH = 70.00;
    public double limitZH = 60.00;
    public double limitXL = -70.00;
    public double limitZL = -60.00;

    private double wiiIndOriginX = 0.0;
    private double wiiIndOriginZ = 0.0;
    private double wiiIndRotZ = 0.0;
    private double wiiIndRotX = 0.0;
    private bool wiiIndA = false;

    public GameObject cursor;

    void Awake() {
        //add a copy of TCPConnection to this game object
        myTCP = gameObject.AddComponent<TCPConnection>();
    }

    void Start() {
        //if connection has not been made, display button to connect
        if(myTCP.socketReady == false) {
            Debug.Log("Attempting to connect..");
            myTCP.setupSocket();
            Debug.Log("connected");
        }
        StartCoroutine(YieldingWork());
    }

    IEnumerator YieldingWork() {
        bool workDone = false;
        while(!workDone) {
            SocketResponse();
            yield return null;
        }
    }

    void Update() {
        if(this.wiiIndA) {
            //Set the main Color of the Material to green
            this.wiiIndicator.GetComponent<Renderer>().material.color = Color.green;
        } else {
            this.wiiIndicator.GetComponent<Renderer>().material.color = Color.red;
        }

        if(Input.GetKeyDown(KeyCode.C)) {
            this.wiiIndOriginX = this.wiiIndRotX;
            this.wiiIndOriginZ = this.wiiIndRotZ;
        }

        /*this.wiiIndicator.transform.Rotate(new Vector3(
            (float)(this.wiiIndRotX - this.wiiIndOriginX),
            this.wiiIndicator.transform.rotation.y,
            this.wiiIndicator.transform.rotation.z
            ));*/

        Vector3 startMarker = this.cursor.transform.position;

        float nextRotX = (float)(this.wiiIndRotX - this.wiiIndOriginX);
        float nextPosY = (float)-this.wiiIndRotX / 10;
        if((!(nextRotX < this.deadzoneXL || this.deadzoneXH < nextRotX)) || 
            (nextRotX < this.limitXL || this.limitXH < nextRotX)) {
            nextRotX = 0;
            nextPosY = 0;
        }

        float nextRotZ = (float)-(this.wiiIndRotZ - this.wiiIndOriginZ);
        float nextPosX = (float)this.wiiIndRotZ / 10;
        if((!(nextRotZ < this.deadzoneZL) && !(this.deadzoneZH < nextRotZ)) ||
            (nextRotZ < this.limitZL || this.limitZH < nextRotZ)) {
            nextRotZ = 0;
            nextPosX = 0;
        }

        Quaternion nextRotation = Quaternion.Euler(nextRotX, 0, nextRotZ);
        this.transform.rotation = Quaternion.Slerp(transform.rotation, nextRotation, Time.deltaTime * this.smooth / 2);

        Vector3 endMarker = startMarker + new Vector3(nextPosX, nextPosY, 0);
        this.cursor.transform.position = Vector3.Lerp(startMarker, endMarker, Time.deltaTime * this.smooth);

        //keep checking the server for messages, if a message is received from server, it gets logged in the Debug console (see function below)
    }

    //socket reading script
    void SocketResponse() {
        string serverSays = myTCP.readSocket();
        String[] data;

        if(serverSays != "") {
            Debug.Log("[SERVER]" + serverSays);
            data = serverSays.Split(';');
            this.wiiIndA = Int32.Parse(data[0]) != 0;
            this.wiiIndRotZ = Double.Parse(data[1]);
            this.wiiIndRotX = Double.Parse(data[2]);
            Debug.Log("A : " + this.wiiIndA + " rot Z : " + this.wiiIndRotZ + " rot X : " + this.wiiIndRotX);
        }
    }

    //send message to the server
    public void SendToServer(string str) {
        myTCP.writeSocket(str);
        Debug.Log("[CLIENT] -> " + str);
    }
}