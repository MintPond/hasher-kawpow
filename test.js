"use strict";

const kawpow = require('./index.js');

process.title = 'verify-test';

const expectedMixHash = '56ea99f4e653df0eda8cbf669e68d2f98d2811ccc4c9aaf1012d11c34505b7a9';
const expectedHash = 'd7b547bee828029701c7f1766c42f9cc5a9013c8e16d24a1854334d9fcef17b9';

const headerHashBuf = Buffer.alloc(32, 1);
const nonceBuf = Buffer.alloc(8, 0);
nonceBuf[0] = 1;

const mixOutBuf = Buffer.alloc(32);
const hashOutBuf = Buffer.alloc(32);

kawpow.hashOne(headerHashBuf, nonceBuf, 10, mixOutBuf, hashOutBuf);

const mixHash = mixOutBuf.toString('hex');
const hash = hashOutBuf.toString('hex');

console.log(`Mix Hash: ${mixHash}`);
if (mixHash !== expectedMixHash)
    throw new Error(`Got invalid mix hash. Expected ${expectedMixHash}`);

console.log(`Hash: ${hash}`);
if (hash !== expectedHash)
    throw new Error(`Got invalid hash. Expected ${expectedHash}`);

// Verify

const mixHashBuf = Buffer.from(mixHash, 'hex');
const verifyHashOutBuf = Buffer.alloc(32);

const isValid = kawpow.verify(headerHashBuf, nonceBuf, 10, mixHashBuf, verifyHashOutBuf);

console.log(`isValid: ${isValid}`);
if (!isValid)
    throw new Error('Expected valid result but result is invalid.');

const verifiedHash = verifyHashOutBuf.toString('hex');
console.log(`Verified Hash: ${verifiedHash}`)
if (verifiedHash !== hash)
    throw new Error(`Verified hash output does not match original hash.`);

