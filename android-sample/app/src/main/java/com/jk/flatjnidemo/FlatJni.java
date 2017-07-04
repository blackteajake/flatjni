package com.jk.flatjnidemo;

/*********************************************************************
 * This file is part of seeyoutime project
 * Created by zhaojian@seeyoutime.com on 6/29/17.
 * Copyright (c) 2016 XingDian Co.,Ltd. - All Rights Reserved
 *********************************************************************/
@SuppressWarnings("JniMissingFunction")
public class FlatJni {
    static {
        System.loadLibrary("flatjni");
    }
    public static native int _call();
}
