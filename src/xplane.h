/*
 * xplane.h
 *
 *  Created on: Feb 13, 2010
 *      Author: max
 */

#ifndef XPLANE_H_
#define XPLANE_H_

#if defined LIN || defined APL
  #if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__ ((visibility("default")))
    #define DLL_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define DLL_PUBLIC
    #define DLL_LOCAL
  #endif
#else
    #define DLL_PUBLIC
    #define DLL_LOCAL
#endif

PLUGIN_API int  DLL_PUBLIC XPluginStart(char *outName,char *outSig,char *outDesc);
PLUGIN_API void DLL_PUBLIC XPluginStop(void);
PLUGIN_API void DLL_PUBLIC XPluginDisable(void);
PLUGIN_API int  DLL_PUBLIC XPluginEnable(void);
PLUGIN_API void DLL_PUBLIC XPluginReceiveMessage(XPLMPluginID inFrom,long inMsg,void *inParam);


#endif /* XPLANE_H_ */
