#pragma once
class TestScenario {
	virtual bool run();
};
class FullWriteAndReadCompare : public TestScenario
{
	bool run() override;
};
class PartialLBAWrite : public TestScenario
{
	bool run() override;
};
class WriteReadAging : public TestScenario
{
	bool run() override;
};
