import unittest
import kawpow

class TestEthash(unittest.TestCase):
    block_height = 2543241
    nonce = 0xc9b1000029c69813
    header_hash = bytes.fromhex(
        '0565cd49085a5b50b783aa1ba12b7fc73fffd6dc5345de1fb5389b0977e4caa2')
    mix_hash = bytes.fromhex(
        '7e8b16f02604df24780ddcc56297fa66c062c2db26a8d54e102ef8c53111e15b')
    final_hash = bytes.fromhex(
        '3755265d35ae6bd9b205850a6b9166a67cd7880bd49bc5ba832d21c583ff90d4')

    def test_hash(self):
        f, m = kawpow.pow(self.header_hash, int.to_bytes(self.nonce, 8, 'little', signed=False), 2543241)
        self.assertEqual(m, self.mix_hash)
        self.assertEqual(f, self.final_hash)