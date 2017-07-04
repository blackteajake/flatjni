// automatically generated by the FlatBuffers compiler, do not modify

package com.jk.greeter;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

@SuppressWarnings("unused")
public final class PlayRequest extends Table {

  static { System.loadLibrary("Greeter"); }

  public static PlayResult play(String path) {
    FlatBufferBuilder builder = new FlatBufferBuilder(0);
    builder.finish(createPlayRequest(builder, builder.createString(path)));
    byte[] reply = _play(builder.dataBuffer().array());
    return PlayResult.getRootAsPlayResult(ByteBuffer.wrap(reply));
  }
  private static native byte[] _play(byte[] req);

  public static PlayRequest getRootAsPlayRequest(ByteBuffer _bb) { return getRootAsPlayRequest(_bb, new PlayRequest()); }
  public static PlayRequest getRootAsPlayRequest(ByteBuffer _bb, PlayRequest obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; }
  public PlayRequest __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public String path() { int o = __offset(4); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer pathAsByteBuffer() { return __vector_as_bytebuffer(4, 1); }

  public static int createPlayRequest(FlatBufferBuilder builder,
      int pathOffset) {
    builder.startObject(1);
    PlayRequest.addPath(builder, pathOffset);
    return PlayRequest.endPlayRequest(builder);
  }

  public static void startPlayRequest(FlatBufferBuilder builder) { builder.startObject(1); }
  public static void addPath(FlatBufferBuilder builder, int pathOffset) { builder.addOffset(0, pathOffset, 0); }
  public static int endPlayRequest(FlatBufferBuilder builder) {
    int o = builder.endObject();
    return o;
  }
}

