#ifndef I_BASE_PROGRAM_H_
#define I_BASE_PROGRAM_H_

class IBaseProgram {
public:
    IBaseProgram() = default;
    virtual ~IBaseProgram() = default;

    virtual void initProgram() = 0;
    virtual void runProgram() = 0;
};

#endif /* End: I_BASE_PROGRAM_H_ */