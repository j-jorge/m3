package com.stuffomatic.apps.m3;

import com.stuffomatic.event.EventManager;

public class Application extends android.app.Application {

    @Override
    public void onCreate() 
    {
        super.onCreate();

        EventManager.initialize( this );
    }
}
