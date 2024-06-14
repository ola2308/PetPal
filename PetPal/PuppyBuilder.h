#pragma once
#include "Puppy.h"

class PuppyBuilder {
protected:
    Puppy* puppy;
public:
    PuppyBuilder() : puppy(new Puppy()) {}
    virtual ~PuppyBuilder() { delete puppy; }
    virtual void buildHappyTexture() = 0;
    virtual void buildSadTexture() = 0;
    virtual void buildSleepTexture() = 0;
    Puppy* getPuppy() const { return puppy; }
};

class BordiBuilder : public PuppyBuilder {
public:
    void buildHappyTexture() override;
    void buildSadTexture() override;
    void buildSleepTexture() override; 
};

class MaltiBuilder : public PuppyBuilder {
public:
    void buildHappyTexture() override;
    void buildSadTexture() override;
    void buildSleepTexture() override; 
};

class GoldiBuilder : public PuppyBuilder {
public:
    void buildHappyTexture() override;
    void buildSadTexture() override;
    void buildSleepTexture() override;
};
