#pragma once
#include <memory>
#include <vector>
#include <functional>

class PropertyNotifierConnection
{
public:
    virtual void disconnect() =  0;
    virtual bool connected() =  0;
    virtual ~PropertyNotifierConnection() {}
};

typedef std::shared_ptr<PropertyNotifierConnection> PropertyNotifierConnectionPtr;
typedef std::weak_ptr<PropertyNotifierConnection> PropertyNotifierConnectionWPtr;
typedef std::function<void()> PropertyNotifierFunction;

class PropertyNotifier
{
public: 
    virtual PropertyNotifierConnectionWPtr connect(PropertyNotifierFunction) = 0;   
    virtual ~PropertyNotifier() {}
};

template<class TFxn> 
class PropertyNotifierConnectionImpl : public PropertyNotifierConnection, 
                                       public std::enable_shared_from_this<PropertyNotifierConnectionImpl<TFxn>>  
{
private:
    TFxn fxn;
    std::weak_ptr<std::vector<std::shared_ptr<PropertyNotifierConnectionImpl<TFxn>>>> connections;
    bool isConnected;

public:
    PropertyNotifierConnectionImpl(TFxn fxn, std::shared_ptr<std::vector<std::shared_ptr<PropertyNotifierConnectionImpl<TFxn>>>> connections)
    {
        this->fxn = fxn;
        this->connections = connections;
        isConnected = false;
    }

    TFxn getFxn()
    {
        return fxn;
    }

    void connect()
    {
        std::shared_ptr<std::vector<std::shared_ptr<PropertyNotifierConnectionImpl<TFxn>>>> conns = connections.lock();
        if (conns)
        {
            conns->emplace_back(shared_from_this());
            isConnected = true;
        }
    }

    bool connected()
    {
        return isConnected;
    }

    void disconnect()
    {
        std::shared_ptr<std::vector<std::shared_ptr<PropertyNotifierConnectionImpl<TFxn>>>> conns = connections.lock();
        if (conns)
        {
            for(auto connection = conns->begin(); connection != conns->end(); connection++)
            {   
                if(*connection == shared_from_this())
                {
                    conns->erase(connection);
                    break;
                }
            }
        }

        isConnected = false;
    }
};

template<class TFxn> struct PropertyNotifierConnectionImplPtr
{
    typedef std::shared_ptr<PropertyNotifierConnectionImpl<TFxn>> Type;
};

template<class TFxn> struct PropertyNotifierConnectionImplVtr
{
    typedef std::vector<typename PropertyNotifierConnectionImplPtr<TFxn>::Type> Type;     
};

template<class TFxn> struct PropertyNotifierConnectionImplVtrPtr
{
    typedef std::shared_ptr<typename PropertyNotifierConnectionImplVtr<TFxn>::Type> Type;     
};

class PropertyNotifierImpl : virtual public PropertyNotifier, 
                             public std::enable_shared_from_this<PropertyNotifierImpl>
{
private:
    PropertyNotifierConnectionImplVtrPtr<PropertyNotifierFunction>::Type fxnConnections;

public: 
    PropertyNotifierImpl() : fxnConnections(new PropertyNotifierConnectionImplVtr<PropertyNotifierFunction>::Type())
    {

    }

    PropertyNotifierConnectionWPtr connect(PropertyNotifierFunction fxn)
    {           
        PropertyNotifierConnectionImplPtr<PropertyNotifierFunction>::Type connection(new PropertyNotifierConnectionImpl<PropertyNotifierFunction>(fxn,fxnConnections));
        connection->connect();
        return std::dynamic_pointer_cast<PropertyNotifierConnection>(connection);
    }

    void fire()
    {
        // Take a copy of the vector so that, while iterating over the 
        // connections, one isn't destroyed, leading to a null-pointer 
        // exception
        auto fxnConnectionsCopy = *fxnConnections;
        for (auto fxnConnection : fxnConnectionsCopy)
        {
            if (fxnConnection->connected())
            {
                fxnConnection->getFxn()();
            }
        }
    }

    bool isEmpty()
    {
        return fxnConnections->empty();
    }
};

class UnifiedBusinessObjectObserver
{
public:
    virtual ~UnifiedBusinessObjectObserver(){};
    virtual void OnInfoChanged() = 0;
};

class UnifiedBusinessObjectNotifiers
{
public:
    virtual ~UnifiedBusinessObjectNotifiers(){};
    virtual std::shared_ptr<PropertyNotifier> getGuidNotifier() =0;
};

class UnifiedBusinessObjectNotifiersImpl : virtual public UnifiedBusinessObjectNotifiers,
                                           virtual public UnifiedBusinessObjectObserver,
                                           public std::enable_shared_from_this<UnifiedBusinessObjectNotifiersImpl>
{
public:
    std::shared_ptr<PropertyNotifier> getGuidNotifier()
    {
        if(!guidNotifier)
        {
            guidNotifier.reset(new PropertyNotifierImpl());
        }
        return std::dynamic_pointer_cast<PropertyNotifier>(guidNotifier);          
    }

    void OnInfoChanged()
    {
        if(guidNotifier)
        {
            guidNotifier->fire(); 
        }
    }

    void OnGuidChanged()
    {
        if(guidNotifier)
        {
            guidNotifier->fire(); 
        }
    }

private:
    std::shared_ptr<PropertyNotifierImpl> guidNotifier;
};

class UnifiedBusinessObject
{
public:
    virtual ~UnifiedBusinessObject(){};
    virtual void addObserver(std::weak_ptr<UnifiedBusinessObjectObserver> observer) = 0;
    virtual void removeObserver(std::weak_ptr<UnifiedBusinessObjectObserver> observer) = 0;
    virtual std::shared_ptr<UnifiedBusinessObjectNotifiers> getUnifiedBusinessObjectNotifiers() = 0;
};

typedef std::shared_ptr<UnifiedBusinessObjectObserver> UnifiedBusinessObjectObserverPtr;
typedef std::weak_ptr<UnifiedBusinessObjectObserver> UnifiedBusinessObjectObserverWPtr;
typedef std::vector<UnifiedBusinessObjectObserverPtr> UnifiedBusinessObjectObserverPtrVtr;
typedef std::vector<UnifiedBusinessObjectObserverWPtr> UnifiedBusinessObjectObserverWPtrVtr;

template<class TObserver> struct ObserverPtr
{
    typedef std::shared_ptr<TObserver> Type;
};

template<class TObserver> struct ObserverPtrVtr
{
    typedef std::vector<typename ObserverPtr<TObserver>::Type> Type;        
};

template <class TObserver> 
typename ObserverPtrVtr<TObserver>::Type filterObservers(UnifiedBusinessObjectObserverWPtrVtr& observers)
{
    typename ObserverPtrVtr<TObserver>::Type filteredObservers;
    size_t i = 0;
    size_t endIndex = observers.size();

    while (i < endIndex)
    {
        if (UnifiedBusinessObjectObserverPtr observer = observers[i].lock())
        {
            // only add to the list if lockedobserver is an instance of type, or one of types children
            typename ObserverPtr<TObserver>::Type filteredObserver = std::dynamic_pointer_cast<TObserver>(observer);
            if (filteredObserver)
            {
                filteredObservers.push_back(filteredObserver);
            }
            i++;
        }
        else
        {
            // Move invalid observers to the end of the vector
            endIndex--;
            std::swap(observers[i], observers[endIndex]);
        }
    }

    // Invalid observers will be erased
    observers.resize(endIndex);

    return filteredObservers;
}

class UnifiedBusinessObjectImpl : virtual public UnifiedBusinessObject
{
public:
    virtual void initNotifiers() = 0;

    void addObserver(std::weak_ptr<UnifiedBusinessObjectObserver> observer){
        observers.push_back(observer);
    }

    void removeObserver(std::weak_ptr<UnifiedBusinessObjectObserver> observer){
        if(UnifiedBusinessObjectObserverPtr observerLock = observer.lock()){
            for (size_t i=0; i!=observers.size(); i++)
            {
                if(UnifiedBusinessObjectObserverPtr observersItemLock = observers[i].lock())
                {
                    if (observersItemLock == observerLock)
                    {
                        observers.erase(observers.begin()+i, observers.begin()+1+i);
                        break;
                    }
                }
            }
        }
    }

    virtual std::shared_ptr<UnifiedBusinessObjectNotifiers> getUnifiedBusinessObjectNotifiers() {
        initNotifiers();
        return std::dynamic_pointer_cast<UnifiedBusinessObjectNotifiers>(notifiers);
    }

protected:
    template <class TObserver> 
    typename ObserverPtrVtr<TObserver>::Type getFilteredObservers()
    {
        return filterObservers<TObserver>(observers);
    }   

protected:
    std::shared_ptr<UnifiedBusinessObjectNotifiersImpl> notifiers;
    UnifiedBusinessObjectObserverWPtrVtr observers;
};
