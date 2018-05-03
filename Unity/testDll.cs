using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Net.Sockets;
using System.Threading;
public class testDll : MonoBehaviour
{
    //variables

    private bool _threadRunning;
    private Thread _thread;
    private TCPConnection myTCP;

    private string serverMsg;

    public string msgToServer;





    void Awake()
    {

        //add a copy of TCPConnection to this game object

        myTCP = gameObject.AddComponent<TCPConnection>();

    }



    void Start()
    {

        //if connection has not been made, display button to connect

        if (myTCP.socketReady == false)
        {



                Debug.Log("Attempting to connect..");

                myTCP.setupSocket();

                Debug.Log("connected");


        }
        StartCoroutine(YieldingWork());

    }
    IEnumerator YieldingWork()
    {
        bool workDone = false;

        while (!workDone)
        {
            SocketResponse();
            yield return null;

        }
    }

    void Update()
    {



        //keep checking the server for messages, if a message is received from server, it gets logged in the Debug console (see function below)




    }



    //socket reading script

    void SocketResponse()
    {

        string serverSays = myTCP.readSocket();

        if (serverSays != "")
        {

            Debug.Log("[SERVER]" + serverSays);

        }



    }



    //send message to the server

    public void SendToServer(string str)
    {

        myTCP.writeSocket(str);

        Debug.Log("[CLIENT] -> " + str);

    }



}