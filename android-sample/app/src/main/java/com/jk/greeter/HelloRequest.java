// automatically generated by the FlatBuffers compiler, do not modify

package com.jk.greeter;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

@SuppressWarnings("unused")
public final class HelloRequest extends Table {
  public static HelloRequest getRootAsHelloRequest(ByteBuffer _bb) { return getRootAsHelloRequest(_bb, new HelloRequest()); }
  public static HelloRequest getRootAsHelloRequest(ByteBuffer _bb, HelloRequest obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; }
  public HelloRequest __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public String message() { int o = __offset(4); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer messageAsByteBuffer() { return __vector_as_bytebuffer(4, 1); }

  public static int createHelloRequest(FlatBufferBuilder builder,
      int messageOffset) {
    builder.startObject(1);
    HelloRequest.addMessage(builder, messageOffset);
    return HelloRequest.endHelloRequest(builder);
  }

  public static void startHelloRequest(FlatBufferBuilder builder) { builder.startObject(1); }
  public static void addMessage(FlatBufferBuilder builder, int messageOffset) { builder.addOffset(0, messageOffset, 0); }
  public static int endHelloRequest(FlatBufferBuilder builder) {
    int o = builder.endObject();
    return o;
  }
}

