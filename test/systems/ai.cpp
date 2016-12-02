#include "systems/ai.hpp"

#include <gtest/gtest.h>
#include <anax/anax.hpp>
#include <string>

using namespace systems;

class TestNode : public AiNode {
public:
    TestNode( std::string name, AiResult result, std::string &outputstring):name(name),result(result),outputstring(outputstring){}

    virtual AiResult _execute(anax::Entity entity, AiSystem const& aisystem){
        outputstring.append(name);
        outputstring.append("E\n");
        return result;
    }
    virtual void _success(anax::Entity entity, AiSystem const& aisystem){
        outputstring.append(name);
        outputstring.append("S\n");
    }
    virtual void _failure(anax::Entity entity, AiSystem const& aisystem){
        outputstring.append(name);
        outputstring.append("F\n");
    }
    const std::string name;
    AiResult result;
    std::string &outputstring;
};

class AiNodeSequenceTest : public testing::Test {
protected:
    virtual void SetUp() {}
    anax::Entity entity;
    AiSystem sys;
    AiNodeSequence a;
    std::string outputstring;
};

/* Test that empty returns success */
TEST_F(AiNodeSequenceTest, sequence_empty)
{

    ASSERT_EQ(AI_RESULT_SUCCESS, a.execute(entity, sys));
    ASSERT_EQ("", outputstring);
    ASSERT_EQ(0, a.get_child_count());
}

/* Test that ready returns ready */
TEST_F(AiNodeSequenceTest, sequence_ready)
{
    a.add_child(new TestNode("A", AI_RESULT_READY, outputstring));
    ASSERT_EQ(AI_RESULT_READY, a.execute(entity, sys));
    ASSERT_EQ(AI_RESULT_READY, a.execute(entity, sys));
    ASSERT_EQ(AI_RESULT_READY, a.execute(entity, sys));
    ASSERT_EQ("AE\nAE\nAE\n", outputstring);
    ASSERT_EQ(1, a.get_child_count());
}

/* Test that success returns success if empty and calls success*/
TEST_F(AiNodeSequenceTest, sequence_success){
    a.add_child(new TestNode("A", AI_RESULT_SUCCESS, outputstring));
    ASSERT_EQ(AI_RESULT_SUCCESS, a.execute(entity, sys));
    ASSERT_EQ("AE\nAS\n", outputstring);
    ASSERT_EQ(0, a.get_child_count());
}

/* Test that returns ready if child node does */
TEST_F(AiNodeSequenceTest, sequence_ready_success){
    auto p_node = new TestNode("A", AI_RESULT_READY, outputstring);
    a.add_child(p_node);
    ASSERT_EQ(AI_RESULT_READY, a.execute(entity, sys));
    ASSERT_EQ("AE\n", outputstring);
    p_node->result = AI_RESULT_SUCCESS;
    ASSERT_EQ(AI_RESULT_SUCCESS, a.execute(entity, sys));
    outputstring = "";
    ASSERT_EQ(0, a.get_child_count());
}

/* Test that add_child increments child count */
TEST_F(AiNodeSequenceTest, sequence_child_count){
    ASSERT_EQ(0, a.get_child_count());
    a.add_child(new TestNode("A", AI_RESULT_SUCCESS, outputstring));
    ASSERT_EQ(1, a.get_child_count());
    a.add_child(new TestNode("B", AI_RESULT_SUCCESS, outputstring));
    ASSERT_EQ(2, a.get_child_count());
    a.add_child(new TestNode("C", AI_RESULT_SUCCESS, outputstring));
    ASSERT_EQ(3, a.get_child_count());
}

/* Test that series of successes  returns ready until final success */
TEST_F(AiNodeSequenceTest, sequence){
    a.add_child(new TestNode("A", AI_RESULT_SUCCESS, outputstring));
    a.add_child(new TestNode("B", AI_RESULT_SUCCESS, outputstring));
    a.add_child(new TestNode("C", AI_RESULT_SUCCESS, outputstring));

    ASSERT_EQ(3, a.get_child_count());
    ASSERT_EQ(AI_RESULT_READY, a.execute(entity, sys));
    ASSERT_EQ(AI_RESULT_READY, a.execute(entity, sys));
    ASSERT_EQ(AI_RESULT_SUCCESS, a.execute(entity, sys));
    ASSERT_EQ("AE\nAS\nBE\nBS\nCE\nCS\n", outputstring);
    ASSERT_EQ(0, a.get_child_count());
}

/*
 * Test that failure results in no further nodes being run
 * and failure being returned
 */
TEST_F(AiNodeSequenceTest, sequence_mid_sequence_faisequencel){
    a.add_child(new TestNode("A", AI_RESULT_SUCCESS, outputstring));
    a.add_child(new TestNode("B", AI_RESULT_SUCCESS, outputstring));
    a.add_child(new TestNode("C", AI_RESULT_FAIL, outputstring));
    a.add_child(new TestNode("D", AI_RESULT_SUCCESS, outputstring));
    ASSERT_EQ(AI_RESULT_READY, a.execute(entity, sys));
    ASSERT_EQ(AI_RESULT_READY, a.execute(entity, sys));
    ASSERT_EQ(AI_RESULT_FAIL, a.execute(entity, sys));
    ASSERT_EQ(AI_RESULT_FAIL, a.execute(entity, sys));
    ASSERT_EQ("AE\nAS\nBE\nBS\nCE\nCF\nCE\nCF\n", outputstring);
    ASSERT_EQ(2, a.get_child_count());
}

class AiNodeSelectorTest : public testing::Test {
protected:
    virtual void SetUp() {}
    anax::Entity entity;
    AiSystem sys;
    AiNodeSelector a;
    std::string outputstring;
};

/* Test that empty returns failure */
TEST_F(AiNodeSelectorTest, sequence_empty)
{

    ASSERT_EQ(AI_RESULT_FAIL, a.execute(entity, sys));
    ASSERT_EQ("", outputstring);
    ASSERT_EQ(0, a.get_child_count());
}

/* Test that ready returns ready */
TEST_F(AiNodeSelectorTest, sequence_ready)
{
    a.add_child(new TestNode("A", AI_RESULT_READY, outputstring));
    ASSERT_EQ(AI_RESULT_READY, a.execute(entity, sys));
    ASSERT_EQ(AI_RESULT_READY, a.execute(entity, sys));
    ASSERT_EQ(AI_RESULT_READY, a.execute(entity, sys));
    ASSERT_EQ("AE\nAE\nAE\n", outputstring);
    ASSERT_EQ(1, a.get_child_count());
}

/* Test that returns success if single node returns success*/
TEST_F(AiNodeSelectorTest, sequence_success){
    a.add_child(new TestNode("A", AI_RESULT_SUCCESS, outputstring));
    ASSERT_EQ(AI_RESULT_SUCCESS, a.execute(entity, sys));
    ASSERT_EQ("AE\nAS\n", outputstring);
    ASSERT_EQ(1, a.get_child_count());
}

/* Test that returns ready if child node does */
TEST_F(AiNodeSelectorTest, sequence_ready_success){
    auto p_node = new TestNode("A", AI_RESULT_READY, outputstring);
    a.add_child(p_node);
    ASSERT_EQ(AI_RESULT_READY, a.execute(entity, sys));
    ASSERT_EQ("AE\n", outputstring);
    p_node->result = AI_RESULT_SUCCESS;
    ASSERT_EQ(AI_RESULT_SUCCESS, a.execute(entity, sys));
    outputstring = "";
    ASSERT_EQ(1, a.get_child_count());
}

/* Test that add_child increments child count */
TEST_F(AiNodeSelectorTest, sequence_child_count){
    ASSERT_EQ(0, a.get_child_count());
    a.add_child(new TestNode("A", AI_RESULT_SUCCESS, outputstring));
    ASSERT_EQ(1, a.get_child_count());
    a.add_child(new TestNode("B", AI_RESULT_SUCCESS, outputstring));
    ASSERT_EQ(2, a.get_child_count());
    a.add_child(new TestNode("C", AI_RESULT_SUCCESS, outputstring));
    ASSERT_EQ(3, a.get_child_count());
}

/* Test that series of failures  returns ready until final failure */
TEST_F(AiNodeSelectorTest, sequence){
    a.add_child(new TestNode("A", AI_RESULT_FAIL, outputstring));
    a.add_child(new TestNode("B", AI_RESULT_FAIL, outputstring));
    a.add_child(new TestNode("C", AI_RESULT_FAIL, outputstring));

    ASSERT_EQ(3, a.get_child_count());
    ASSERT_EQ(AI_RESULT_READY, a.execute(entity, sys));
    ASSERT_EQ(AI_RESULT_READY, a.execute(entity, sys));
    ASSERT_EQ(AI_RESULT_FAIL, a.execute(entity, sys));
    ASSERT_EQ("AE\nAF\nBE\nBF\nCE\nCF\n", outputstring);
    ASSERT_EQ(0, a.get_child_count());
}

/*
 * Test that success results in no further nodes being run
 * and success being returned
 */
TEST_F(AiNodeSelectorTest, sequence_mid_sequence_success){
    a.add_child(new TestNode("A", AI_RESULT_FAIL, outputstring));
    a.add_child(new TestNode("B", AI_RESULT_FAIL, outputstring));
    a.add_child(new TestNode("C", AI_RESULT_SUCCESS, outputstring));
    a.add_child(new TestNode("D", AI_RESULT_FAIL, outputstring));
    ASSERT_EQ(AI_RESULT_READY, a.execute(entity, sys));
    ASSERT_EQ(AI_RESULT_READY, a.execute(entity, sys));
    ASSERT_EQ(AI_RESULT_SUCCESS, a.execute(entity, sys));
    ASSERT_EQ(AI_RESULT_SUCCESS, a.execute(entity, sys));
    ASSERT_EQ("AE\nAF\nBE\nBF\nCE\nCS\nCE\nCS\n", outputstring);
    ASSERT_EQ(2, a.get_child_count());
}
