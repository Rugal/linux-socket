#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        int socket;
        unsigned int address;
        unsigned int port;
        unsigned int queue;
    } Configuration;

    Configuration* createConfiguration();
    void deleteConfiguration(Configuration* conf);

#ifdef __cplusplus
}
#endif

#endif /* CONFIGURATION_H */
