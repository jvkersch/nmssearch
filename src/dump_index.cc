// Utility command to dump a HNSW to JSON for easy inspection. This is done by directly
// reading the serialized format written by NMSLIB. This format is not documented, however, 
// and may change between different versions of NMSLIB. When updating the vendorized 
// version of NMSLIB, check that the type and order of the fields in `readNodeData` 
// matches what NMSLIB writes.

#include <filesystem>
#include <ios>
#include <iostream>
#include <vector>

// nmslib
#include "utils.h"

// local
#include "dump_index.h"
#include "errors.h"

namespace fs = std::filesystem;

struct LightWeightNode
{
    unsigned int id;
    unsigned int level;
    std::vector<unsigned int> friends;

    LightWeightNode(unsigned int id, unsigned int level, std::vector<unsigned int> friends)
        : id(id), level(level), friends(friends) {}
};

std::vector<LightWeightNode> readNodeData(fs::path fileName)
{

    std::ifstream input(fileName, std::ios::binary);

    unsigned int optimIndexFlag = 0;
    similarity::readBinaryPOD(input, optimIndexFlag);
    if (optimIndexFlag != 0)
    {
        throw ExitError("Can only dump unoptimized indices.");
    }

    unsigned int totalElementsStored_;
    int maxlevel_;
    unsigned int enterpointId_;
    size_t M_, maxM_, maxM0_;

    similarity::readBinaryPOD(input, totalElementsStored_);
    similarity::readBinaryPOD(input, maxlevel_);
    similarity::readBinaryPOD(input, enterpointId_);
    similarity::readBinaryPOD(input, M_);
    similarity::readBinaryPOD(input, maxM_);
    similarity::readBinaryPOD(input, maxM0_);

    std::vector<LightWeightNode> nodes;
    for (unsigned id = 0; id < totalElementsStored_; ++id)
    {
        unsigned currlevel;
        similarity::readBinaryPOD(input, currlevel);

        std::vector<unsigned int> friends;
        for (unsigned level = 0; level <= currlevel; ++level)
        {
            unsigned friendQty;
            similarity::readBinaryPOD(input, friendQty);
            friends.resize(friendQty);

            for (unsigned k = 0; k < friendQty; ++k)
            {
                similarity::IdType friendId;
                similarity::readBinaryPOD(input, friendId);
                friends[k] = friendId;
            }
        }
        nodes.push_back({id, currlevel, friends});
    }
    input.close();

    return nodes;
}

void writeOpenBracket()
{
    std::cout << "[" << std::endl;
}

void writeCloseBracket()
{
    std::cout << "\n]" << std::endl;
}

void writeSeparator()
{
    std::cout << "," << std::endl;
}

void writeNode(const LightWeightNode &node)
{
    std::cout << "  {" << std::endl;
    std::cout << "    \"id\": " << node.id << "," << std::endl;
    std::cout << "    \"level\": " << node.level << "," << std::endl;
    std::cout << "    \"friends\": [";
    for (int i = 0; i < node.friends.size() - 1; i++)
    {
        std::cout << node.friends[i] << ", ";
    }
    std::cout << node.friends.back() << "]" << std::endl;
    std::cout << "  }";
}

void writeNodes(const std::vector<LightWeightNode> &nodes)
{
    writeOpenBracket();
    for (int k = 0; k < nodes.size() - 1; k++)
    {
        writeNode(nodes[k]);
        writeSeparator();
    }
    writeNode(nodes.back());
    writeCloseBracket();
}

void DumpIndexCommand::run() const
{
    std::cerr << "Dumping database " << m_params.database_path << std::endl;
    if (m_params.index_algorithm != IndexAlgorithm::hnsw)
    {
        throw ExitError("Can only dump index of type HNSW");
    }

    fs::path index_bin = m_params.database_path / "index.bin";

    auto nodes = readNodeData(index_bin);
    writeNodes(nodes);

    std::cerr << "Wrote data for " << nodes.size() << " node(s)." << std::endl;
}