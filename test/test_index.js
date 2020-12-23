const { clipboardObject } = require("../index.js");
const assert = require("assert");
const path = require("path");
const fs = require("fs");

const CF_TEXT = 1;
const CF_HDROP = 15;
assert(clipboardObject, "预测试的功能未定义-add");

const p = path.join(__dirname, '../dll/clipboardDll.dll')
const obj = clipboardObject(p)
const err = obj.lastErrorInfo();
if (err) {
    console.log(err)
}

function testBasic() {
    // obj.CopyText("qefff啊啊啊");
    // console.log(obj.PasteText());

    obj.CopyFile("C:\\Users\\shenq\\Pictures\\喊啊哈哈哈哈哈哈哈哈哈   啊哈哈哈哈哈哈哈   纠结啊纠结啊就11.xlsx");
    // assert.strictEqual(result, 1333, `返回了其他值${result}`);

    const uFormat = obj.IsClipboardFormat();
    console.log(uFormat);
    if (uFormat == CF_TEXT) {
        const text = obj.PasteText();
        console.log("CF_TEXT", text)
    }
    else if (uFormat == CF_HDROP) {
        const file = obj.PasteFile();
        console.log("CF_HDROP", file)
    }
    else {
        console.log(uFormat)
    }
}

assert.doesNotThrow(testBasic, undefined, `testBasic 抛出异常`);

console.log("测试通过!")