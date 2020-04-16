'use strict'

const kawpow = require('bindings')('hasherkawpow.node');

module.exports = {

    /**
     * Hash using a single nonce and place results into the specified output Buffers.
     *
     * Note that all input values are expected to be in little-endian format.
     *
     * All output values are in little endian format.
     *
     * @param headerHashBuf {Buffer} 32-byte header hash
     * @param nonceBuf {Buffer} 8-byte nonce value (64-bits)
     * @param blockHeight {number} Block height integer
     * @param mixOutBuf {Buffer} Mix hash result output Buffer
     * @param hashOutBuf {Buffer} Hash result output Buffer
     */
    hashOne: hashOne,

    /**
     * Verify a mix hash.
     *
     * Note that all input values are expected to be in little-endian format.
     *
     * All output values are in little endian format.
     *
     * @param headerHashBuf {Buffer} 32-byte header hash
     * @param nonceBuf {Buffer} 8-byte nonce value (64-bits)
     * @param blockHeight {number} Block height integer
     * @param mixHashBuf {Buffer} Mix hash for verification
     * @param hashOutBuf {Buffer} Hash result output Buffer
     * @returns {boolean} True if valid, otherwise false.
     */
    verify: verify
};


function hashOne(headerHashBuf, nonceBuf, blockHeight, mixOutBuf, hashOutBuf) {

    _expectBuffer(headerHashBuf, 'headerHashBuf', 32);
    _expectBuffer(nonceBuf, 'nonceBuf', 8);
    _expectInteger(blockHeight, 'blockHeight');
    _expectBuffer(mixOutBuf, 'mixOutBuf', 32);
    _expectBuffer(hashOutBuf, 'hashOutBuf', 32);

    kawpow.hash_one(headerHashBuf, nonceBuf, blockHeight, mixOutBuf, hashOutBuf);
}


function verify(headerHashBuf, nonceBuf, blockHeight, mixHashBuf, hashOutBuf) {

    _expectBuffer(headerHashBuf, 'headerHashBuf', 32);
    _expectBuffer(nonceBuf, 'nonceBuf', 8);
    _expectInteger(blockHeight, 'blockHeight');
    _expectBuffer(mixHashBuf, 'mixOutBuf', 32);
    _expectBuffer(hashOutBuf, 'hashOutBuf', 32);

    return kawpow.verify(headerHashBuf, nonceBuf, blockHeight, mixHashBuf, hashOutBuf);
}


function _expectBuffer(buffer, name, size) {
    if (!Buffer.isBuffer(buffer))
        throw new Error(`"${name}" is expected to be a Buffer. Got ${(typeof buffer)} instead.`);

    if (size && buffer.length !== size)
        throw new Error(`"${name}" is expected to be exactly ${size} bytes. Got ${buffer.length} instead.`);
}


function _expectInteger(num, name) {
    if (typeof num !== 'number')
        throw new Error(`"${name}" is expected to be a number. Got ${(typeof num)} instead.`);

    if (isNaN(num) || !isFinite(num))
        throw new Error(`"${name}" is not a number.`);

    if (!Number.isInteger(num))
        throw new Error(`"${name}" is expected to be an integer. Got ${num} instead.`);
}