#include "propertynotifier.h"
#include <gmock/gmock.h>

class PhotoNotifiers : virtual public UnifiedBusinessObjectNotifiers {
public:
    virtual std::shared_ptr<PropertyNotifier> getImageTypeNotifier() = 0;
};

class PhotoObserver : virtual public UnifiedBusinessObjectObserver
{
public:
    virtual void OnImageTypeChanged() = 0;
};

class Photo : virtual public UnifiedBusinessObject
{
public:
    virtual void addObserver(std::weak_ptr<PhotoObserver> observer) =0;
    virtual void removeObserver(std::weak_ptr<PhotoObserver> observer) =0;
};

class PhotoNotifiersImpl : public UnifiedBusinessObjectNotifiersImpl, virtual public PhotoNotifiers, virtual public PhotoObserver {
public:
    virtual std::shared_ptr<PropertyNotifier> getImageTypeNotifier() override{
        if(!imageTypeNotifier) {
            imageTypeNotifier.reset(new PropertyNotifierImpl());
        }

        return std::dynamic_pointer_cast<PropertyNotifier>(imageTypeNotifier);
    }

    void OnImageTypeChanged() {
        if(imageTypeNotifier){
            imageTypeNotifier->fire(); 
        }
    }

private:
    std::shared_ptr<PropertyNotifierImpl> imageTypeNotifier;
};

class PhotoBaseImpl : public UnifiedBusinessObjectImpl, public Photo {
public:
    void initNotifiers()
    {
        if (!notifiers)
        {
            notifiers.reset(new PhotoNotifiersImpl());
            addObserver(std::weak_ptr<PhotoObserver>(std::dynamic_pointer_cast<PhotoObserver>(this->notifiers)));
        }
    }

    void fireImageType()
    {
        ObserverPtrVtr<PhotoObserver>::Type observers = getFilteredObservers<PhotoObserver>();
        for (size_t i=0; i!=observers.size(); i++)
        {
            observers[i]->OnImageTypeChanged();
        }
    }

    using UnifiedBusinessObjectImpl::addObserver;
    using UnifiedBusinessObjectImpl::removeObserver;

    void addObserver(std::weak_ptr<PhotoObserver> observer)
    {
        UnifiedBusinessObjectImpl::addObserver(observer);
    }

    void PhotoBaseImpl::removeObserver(std::weak_ptr<PhotoObserver> observer)
    {
        UnifiedBusinessObjectImpl::removeObserver(observer);
    }


};

TEST(Photo, test) {
    PhotoBaseImpl* base = new PhotoBaseImpl();

}


