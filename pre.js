Module['toJSObject'] = function(array) {
    const numBytes = array.length * array.BYTES_PER_ELEMENT;
    const ptr = Module._malloc(numBytes);
    const heapBytes = new Uint8Array(Module.HEAPU8.buffer, ptr, numBytes);
    heapBytes.set(new Uint8Array(array.buffer));
    const ref = Module.getRoot(heapBytes.byteOffset, array.length);
    const object = ref.toJSObject()
    ref.delete()
    Module._free(heapBytes.byteOffset);
    return object
}

Module['fromJSObject'] = function(value) {
    const builder = new Module.Builder();
    const build = function(builder, value) {
        if(value === null || value === undefined) {
            builder.addNull()
            return
        }
        switch(typeof value) {
            case "string": {
                builder.addString(value)
                break
            }
            case "boolean": {
                builder.addBool(value)
                break
            }
            case "number": {
                if(Number.isInteger(value))
                    builder.addInt(value)
                else
                    builder.addDouble(value)
                break
            }
            case "object":{
                if(Array.isArray(value)) {
                    const offset = builder.startVector()
                    for (var i in value) {
                        build(builder, value[i])
                    }
                    builder.endVector(offset, false, false)
                }
                else {
                    const offset = builder.startMap()
                    for (var key in value) {
                        if (value.hasOwnProperty(key)) {
                            builder.addKey(key)
                            build(builder, value[key])

                        }
                    }
                    builder.endMap(offset)
                }
            }
        }
    }
    build(builder, value)
    builder.finish()
    const buffer = builder.buffer.slice()
    builder.delete()
    return buffer
}
