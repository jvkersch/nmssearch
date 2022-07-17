import os.path as op
import shutil
import subprocess
import tempfile
import unittest

# Set this to True to update the test case data in place.
UPDATE_TESTCASES = False


def run_nmssearch(args, **kwds):
    proc = subprocess.run(["../build/nmssearch"] + args, check=True, encoding="utf-8", **kwds)
    return proc


class NMSSearchTestCase:
    def _build(self):
        index_name = op.join(self.d, "index.bin")
        run_nmssearch(self.build_parameters + ["build", self.seqs_file, index_name])
        return index_name

    def _run(self, index_name):
        proc = run_nmssearch(
            self.query_parameters + ["query", index_name, self.query_file],
            stdout=subprocess.PIPE,
        )
        return proc.stdout

    def test_build_then_query(self):
        self.d = d = tempfile.mkdtemp()
        try:
            index_name = self._build()
            output = self._run(index_name)

            if UPDATE_TESTCASES:
                with open(self.output_file, "wt", encoding="utf-8") as fp:
                    fp.write(output)

            with open(self.output_file, encoding="utf-8") as fp:
                expected = fp.read()
            self.assertMultiLineEqual(output, expected)

        finally:
            shutil.rmtree(d)


class TestHNSWAlignSmall(unittest.TestCase, NMSSearchTestCase):

    build_parameters = []

    query_parameters = []

    seqs_file = "data/seqs-small.fa"

    query_file = "data/query-small.fa"

    output_file = "data/output/hnsw-align-small.tsv"


class TestVPTreeAlignSmall(unittest.TestCase, NMSSearchTestCase):

    build_parameters = ["--algorithm", "vptree"]

    query_parameters = ["--algorithm", "vptree"]

    seqs_file = "data/seqs-small.fa"

    query_file = "data/query-small.fa"

    output_file = "data/output/hnsw-align-small.tsv"


class TestBruteforceAlignSmall(unittest.TestCase, NMSSearchTestCase):

    build_parameters = ["--algorithm", "bruteforce"]

    query_parameters = ["--algorithm", "bruteforce"]

    seqs_file = "data/seqs-small.fa"

    query_file = "data/query-small.fa"

    output_file = "data/output/hnsw-align-small.tsv"

    def _build(self):
        return self.seqs_file


class TestHNSWSpectrumSmall(unittest.TestCase, NMSSearchTestCase):

    build_parameters = ["--kmer-length", "2", "--algorithm", "hnsw-kmer"]

    query_parameters = ["--kmer-length", "2", "--algorithm", "hnsw-kmer"]

    seqs_file = "data/seqs-small.fa"

    query_file = "data/query-small.fa"

    output_file = "data/output/hnsw-spectrum-small.tsv"


class TestBruteforceSpectrumSmall(unittest.TestCase, NMSSearchTestCase):

    build_parameters = ["--kmer-length", "2", "--algorithm", "bruteforce-kmer"]

    query_parameters = ["--kmer-length", "2", "--algorithm", "bruteforce-kmer"]

    seqs_file = "data/seqs-small.fa"

    query_file = "data/query-small.fa"

    output_file = "data/output/hnsw-spectrum-small.tsv"


if __name__ == "__main__":
    unittest.main()
