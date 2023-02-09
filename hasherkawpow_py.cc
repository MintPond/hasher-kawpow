#include <Python.h>
#include <stdint.h>
#include "ethash.h"
#include "ethash.hpp"
#include "progpow.hpp"
#include "uint256.h"
#include "helpers.hpp"

static ethash::epoch_context_ptr context{nullptr, nullptr};

static PyObject* pow(PyObject *self, PyObject *args) {
    Py_buffer header_hash_buf, nonce64_buf;
    int block_height;
    if (!PyArg_ParseTuple(args, "y*y*i", &header_hash_buf, &nonce64_buf, &block_height))
        return NULL;

    if (header_hash_buf.len != 32 || nonce64_buf.len != 8) {
        PyErr_SetString(PyExc_ValueError, "Buffer length is not correct");
        return NULL;
    }

    const ethash::hash256* header_hash_ptr = (ethash::hash256*)header_hash_buf.buf;
    uint64_t* nonce64_ptr = (uint64_t*)nonce64_buf.buf;
    ethash::hash256 mix_out;
    ethash::hash256 hash_out;

    const auto epoch_number = ethash::get_epoch_number(block_height);

    if (!context || context->epoch_number != epoch_number)
        context = ethash::create_epoch_context(epoch_number);

    progpow::hash_one(*context, block_height, header_hash_ptr, *nonce64_ptr, &mix_out, &hash_out);

    PyBuffer_Release(&header_hash_buf);
    PyBuffer_Release(&nonce64_buf);

    PyObject *result = PyBytes_FromStringAndSize((const char *)&hash_out, sizeof(ethash::hash256));
    return result;
}

static PyObject *verify(PyObject *self, PyObject *args) {
        Py_buffer header_hash_buf, nonce64_buf, mix_hash_buf, hash_out_buf;
        int block_height;
        static ethash::epoch_context_ptr context{nullptr, nullptr};

        if (!PyArg_ParseTuple(args, "y*y*iy*y*", &header_hash_buf, &nonce64_buf, &block_height, &mix_hash_buf, &hash_out_buf))
            return NULL;

        const ethash::hash256* header_hash_ptr = (ethash::hash256*)header_hash_buf.buf;
        uint64_t* nonce64_ptr = (uint64_t*)nonce64_buf.buf;
        const ethash::hash256* mix_hash_ptr = (ethash::hash256*)mix_hash_buf.buf;
        ethash::hash256* hash_out_ptr = (ethash::hash256*)hash_out_buf.buf;

        const auto epoch_number = ethash::get_epoch_number(block_height);

        if (!context || context->epoch_number != epoch_number)
            context = ethash::create_epoch_context(epoch_number);

        bool is_valid = progpow::verify(*context, block_height, header_hash_ptr, *mix_hash_ptr, *nonce64_ptr, hash_out_ptr);

        PyBuffer_Release(&header_hash_buf);
        PyBuffer_Release(&nonce64_buf);
        PyBuffer_Release(&mix_hash_buf);
        PyBuffer_Release(&hash_out_buf);

        if (is_valid) {
            Py_RETURN_TRUE;
        }
        else {
            Py_RETURN_FALSE;
        }
}

static PyMethodDef methods[] = {
        {"pow", (PyCFunction)pow, METH_VARARGS},
        {"verify", (PyCFunction)verify, METH_VARARGS},
        {NULL, NULL}
};

static struct PyModuleDef module = {
        PyModuleDef_HEAD_INIT,
        "kawpow",
        NULL,
        -1,
        methods
};

PyMODINIT_FUNC PyInit_kawpow(void) {
    return PyModule_Create(&module);
}

