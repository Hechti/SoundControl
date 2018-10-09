package com.example.denny.musiccontrol;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonArrayRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;

import java.io.IOException;
import java.net.Socket;

public class MainActivity extends AppCompatActivity {
    public static final String MESSAGE_IP = "com.example.denny.musiccontrol.IP";
    public static final String MESSAGE_PORT = "com.example.denny.musiccontrol.PORT";
    private RequestQueue requestQueue;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        requestQueue = Volley.newRequestQueue(this);
    }
	
	public void sendMessage(View view) {
        final EditText editText = findViewById(R.id.editText);
        final EditText ipTextField = findViewById(R.id.ipTextField);
        final EditText portTextField = findViewById(R.id.portTextField);

        String url = "http://" + ipTextField.getText().toString() + ":" + portTextField.getText().toString();
        url += "/next";
        JsonArrayRequest arrReq = new JsonArrayRequest(Request.Method.GET, url, null,
                new Response.Listener<JSONArray>() {
                    @Override
                    public void onResponse(JSONArray response) {
                        Log.v("REST", "onResponse");
                    }
                },

                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Log.e("REST", error.toString());
                    }
                }
        );
        // Add the request we just defined to our request queue.
        // The request queue will automatically handle the request as soon as it can.
        requestQueue.add(arrReq);

        Intent intent = new Intent(this, DisplayMessageActivity.class);
        intent.putExtra(MESSAGE_IP, ipTextField.getText().toString());
        intent.putExtra(MESSAGE_IPPORT, portTextField.getText().toString());
        startActivity(intent);
	}
}

class ServerConnection {
    private static ServerConnection instance;
    private Thread thread;
    private ServerConnectionRunnable runnable;
    private String ip;
    private int port;
    private Socket socket;

    public static ServerConnection getInstance() {
        if (instance == null) {
            instance = new ServerConnection();
        }
        return instance;
    }

    private ServerConnection() {
    }

    public boolean connect(final String ip, final int port) {
        Log.v(ServerConnection.class.getName(), "Ip = " + ip + ", port = " + port);
        runnable = new ServerConnectionRunnable(ip, port);
        thread = new Thread(runnable);
        thread.start();
        return runnable.getStatus();
    }

    public void disconnect() {
        thread.stop();
    }
}

class ServerConnectionRunnable implements Runnable
{
    private String ip;
    private int port;
    private Socket socket;
    private boolean status;

    ServerConnectionRunnable(final String ip, final int port) {
        this.ip = ip;
        this.port = port;
        status = false;
    }

    public boolean getStatus() {
        return status;
    }

    @Override
    public void run() {
        try {
            Log.v(ServerConnectionRunnable.class.getName(),"Starte die App");
            socket = new Socket(ip, port);
            status = true;
            Log.v(ServerConnectionRunnable.class.getName(),"Verbindung erfolgreich");
        } catch (IOException e) {
            Log.v(ServerConnectionRunnable.class.getName(), e.getMessage());
            Log.v(ServerConnectionRunnable.class.getName(),"Verbindung nicht erfolgreich");
            status = false;
            e.getStackTrace();
        }
    }
}
