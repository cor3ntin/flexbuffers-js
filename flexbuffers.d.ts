declare module  '@cor3ntin/flexbuffers-wasm' {

class FlexBuffers {
    constructor();
    onRuntimeInitialized : () => void;
    public toJSObject(bytes: Uint8Array) : any;
    public fromJSObject(object: any) : Uint8Array;
}

export = FlexBuffers;

}