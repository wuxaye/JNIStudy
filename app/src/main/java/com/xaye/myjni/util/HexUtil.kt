package com.xaye.myjni.util

/**
 * Author xaye
 * @date: 2024/10/25
 */
object HexUtil {
    fun hexToByteArray(inHex: String): ByteArray {
        // 确保输入十六进制字符串有偶数长度填充‘0’
        val paddedHex = if (inHex.length % 2 == 0) inHex else "0$inHex"
        val result = ByteArray(paddedHex.length / 2)

        // 将每一对十六进制数字转换为一个字节
        for (i in paddedHex.indices step 2) {
            result[i / 2] = hexToByte(paddedHex.substring(i, i + 2))
        }
        return result
    }

    private fun hexToByte(inHex: String): Byte {
        return inHex.toInt(16).toByte()
    }

}