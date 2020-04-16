#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include <stdint.h>
#include <iostream>
#include "nan.h"
#include "include/ethash.h"
#include "include/ethash.hpp"
#include "include/progpow.hpp"
#include "uint256.h"
#include "helpers.hpp"

using namespace node;
using namespace v8;

#define THROW_ERROR_EXCEPTION(x) Nan::ThrowError(x)


NAN_METHOD(hash_one) {

        if (info.Length() < 3)
            return THROW_ERROR_EXCEPTION("hasher-kawpow.hash_one - 3 arguments expected.");

        const ethash::hash256* header_hash_ptr = (ethash::hash256*)Buffer::Data(Nan::To<v8::Object>(info[0]).ToLocalChecked());
        uint64_t* nonce64_ptr = (uint64_t*)Buffer::Data(Nan::To<v8::Object>(info[1]).ToLocalChecked());
        int block_height = info[2]->IntegerValue(Nan::GetCurrentContext()).FromJust();
        ethash::hash256* mix_out_ptr = (ethash::hash256*)Buffer::Data(Nan::To<v8::Object>(info[3]).ToLocalChecked());
        ethash::hash256* hash_out_ptr = (ethash::hash256*)Buffer::Data(Nan::To<v8::Object>(info[4]).ToLocalChecked());

        static ethash::epoch_context_ptr context{nullptr, nullptr};

        const auto epoch_number = ethash::get_epoch_number(block_height);

        if (!context || context->epoch_number != epoch_number)
            context = ethash::create_epoch_context(epoch_number);

        progpow::hash_one(*context, block_height, header_hash_ptr, *nonce64_ptr, mix_out_ptr, hash_out_ptr);
}


NAN_METHOD(verify) {

        if (info.Length() < 5)
            return THROW_ERROR_EXCEPTION("hasher-kawpow.verify - 5 arguments expected.");

        const ethash::hash256* header_hash_ptr = (ethash::hash256*)Buffer::Data(Nan::To<v8::Object>(info[0]).ToLocalChecked());
        uint64_t* nonce64_ptr = (uint64_t*)Buffer::Data(Nan::To<v8::Object>(info[1]).ToLocalChecked());
        int block_height = info[2]->IntegerValue(Nan::GetCurrentContext()).FromJust();
        const ethash::hash256* mix_hash_ptr = (ethash::hash256*)Buffer::Data(Nan::To<v8::Object>(info[3]).ToLocalChecked());
        ethash::hash256* hash_out_ptr = (ethash::hash256*)Buffer::Data(Nan::To<v8::Object>(info[4]).ToLocalChecked());

        static ethash::epoch_context_ptr context{nullptr, nullptr};

        const auto epoch_number = ethash::get_epoch_number(block_height);

        if (!context || context->epoch_number != epoch_number)
            context = ethash::create_epoch_context(epoch_number);

        bool is_valid = progpow::verify(*context, block_height, header_hash_ptr, *mix_hash_ptr, *nonce64_ptr, hash_out_ptr);

        if (is_valid) {
           info.GetReturnValue().Set(Nan::True());
        }
        else {
           info.GetReturnValue().Set(Nan::False());
        }
}

NAN_MODULE_INIT(init) {
        Nan::Set(target, Nan::New("hash_one").ToLocalChecked(), Nan::GetFunction(Nan::New<FunctionTemplate>(hash_one)).ToLocalChecked());
        Nan::Set(target, Nan::New("verify").ToLocalChecked(), Nan::GetFunction(Nan::New<FunctionTemplate>(verify)).ToLocalChecked());
}

NODE_MODULE(hashermtp, init)