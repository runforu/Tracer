#ifndef _RULESMANAGER_H_
#define _RULESMANAGER_H_

#include <string>
#include <vector>
#include <utility>
#include "IRegExp.h"
#include "IPatternCollection.h"

typedef std::pair<std::string, std::string> FunctionGuarder;

class CRouteGuarder
{
    friend class CPatternLoader;

public:
    CRouteGuarder()
    {
    }

    void SetValue( FunctionGuarder & exp )
    {
        m_FuncExp = exp;
    }

    void SetValue( std::string & aEnterExp, std::string & aExitExp )
    {
        m_FuncExp.first = aEnterExp;
        m_FuncExp.second = aExitExp;
        m_bEnabled = true;
    }

    void SetValue( const char * aEnterExp, const char * aExitExp )
    {
        m_FuncExp.first = aEnterExp;
        m_FuncExp.second = aExitExp;
        m_bEnabled = true;
    }

    void Enable( bool enable )
    {
        m_bEnabled = enable;
    }

    const FunctionGuarder& GetValue()
    {
        return m_FuncExp;
    }

    bool IsEnabled()
    {
        return m_bEnabled;
    }

private:
    FunctionGuarder m_FuncExp;
    bool m_bEnabled;
};


class CThreadIdExp
{
    friend class CPatternLoader;

public:
    CThreadIdExp()
    {
    }

    void SetThreadIdExp( const char * exp )
    {
        m_strThreadIDExp = exp;
        m_bEnabled = true;
    }

    void SetThreadIdExp( const std::string & exp )
    {
        m_strThreadIDExp = exp;
        m_bEnabled = true;
    }

    void Enable( bool enable )
    {
        m_bEnabled = enable;
    }

    const std::string& GetThreadIdExp()
    {
        return m_strThreadIDExp;
    }

    bool IsEnabled()
    {
        return m_bEnabled;
    }

private:
    std::string m_strThreadIDExp;
    bool m_bEnabled;
};

/**
* The CPatternLoader mediates between UI and pattern collection
* and mediates between pattern collection and permanent storage.
*/
class CPatternLoader
{
public:    
    CPatternLoader()
    {
    }

    ~CPatternLoader(void)
    {
    }

    unsigned int FuncGuarderCount() const
    {
        return static_cast<unsigned int>( m_Rules.size() );
    }

    unsigned int ThreadIdExpCount() const
    {
        return static_cast<unsigned int>( m_ThreadIDExp.size() );
    }

    void AppendGuarder( CRouteGuarder &rule );

    const CRouteGuarder& GetGuarder(  int index )
    {
        return m_Rules[index];
    }

    void RemoveGuarder( int index );

    void RemoveAll();

    void AppendThreadIdExp( CThreadIdExp& exp );

    const CThreadIdExp& GetThreadIdExp( int index );

    void RemoveThreadIdExp( int index );  

private:
    CPatternLoader( const CPatternLoader& );

    std::vector<CRouteGuarder> m_Rules;
    std::vector<CThreadIdExp> m_ThreadIDExp;
};

#endif //_RULESMANAGER_H_