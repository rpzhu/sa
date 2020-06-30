#ifndef QTPROPERTYIDSTORAGE_H
#define QTPROPERTYIDSTORAGE_H

#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include <QtTreePropertyBrowser>

template<typename ID_TYPE>
class QtPropertyIdStorage
{
public:
	QtPropertyIdStorage()
	{

	}
	~QtPropertyIdStorage()
	{

	}

public:
	void rememberTheProperty(const ID_TYPE& propertyIdName,QtProperty* propertyPtr)
	{
		m_id2Property[propertyIdName] = propertyPtr;
		m_Property2id[propertyPtr] = propertyIdName;
	}
	///
	/// \brief ɾ������propertyָ��
	///
	void deleteAllProperty()
	{
		//ɾ�����м�¼������
		QMapIterator<QtProperty*,ID_TYPE> i(m_Property2id);
		while(i.hasNext())
		{
			i.next();
			delete i.key();
		}
		m_id2Property.clear();
		m_Property2id.clear();
	}
    void deleteProperty(ID_TYPE id)
    {
        QtProperty* pro = m_id2Property[id];
        m_id2Property.remove(id);
        m_Property2id.remove(pro);
    }

	QtProperty* getProperty(const ID_TYPE& propertyIdName) const
	{
		return m_id2Property[propertyIdName];
	}
	//    QString getPropertyID(QtProperty* property) const
	//    {
	//        return m_Property2id[property];
	//    }
	ID_TYPE getPropertyID(QtProperty* property) const
	{
		return m_Property2id[property];
	}
	///
	/// \brief addDoublePropertyInGroup ����Ӷ������������
	/// \param groupItem �������ָ��
	/// \param propertyName ��������ǰ����ʾ������
	/// \param propertyIdName ���Ա�ʶ�����������ظ�
	/// \param propertyData ���Ե�ֵ
	///
	QtProperty* addDoublePropertyInGroup(QtDoublePropertyManager* doubleManage
		,QtProperty* groupItem
		,const QString& propertyName
		,const ID_TYPE& propertyIdName
		,double propertyData)
	{
		QtProperty* subitem = doubleManage->addProperty(propertyName);
		doubleManage->setValue(subitem,propertyData);
		groupItem->addSubProperty(subitem);
		rememberTheProperty(propertyIdName,subitem);
		return subitem;
	}
	QtProperty* addDoublePropertyInGroup(QtVariantPropertyManager* varManage
		,QtProperty* groupItem
		,const QString& propertyName
		,const ID_TYPE& propertyIdName
		,double propertyData)
	{
		QtProperty* subitem = varManage->addProperty(QVariant::Double,propertyName);
		varManage->setValue(subitem,propertyData);
		groupItem->addSubProperty(subitem);
		rememberTheProperty(propertyIdName,subitem);
		return subitem;
	}
	///
	/// \brief property_addVariantPropertyInGroup ����Ӷ������������
	/// \param groupItem �������ָ��
	/// \param propertyName ��������ǰ����ʾ������
	/// \param propertyIdName ���Ա�ʶ�����������ظ�
	/// \param propertyData ���Ե�ֵ
	///
	QtVariantProperty* addVariantPropertyInGroup(QtVariantPropertyManager* VariantManage
		,int nType
		,QtProperty* groupItem
		,const QString& propertyName
		,const ID_TYPE& propertyIdName
		,QVariant propertyData)
	{
		QtVariantProperty* subitem = VariantManage->addProperty(nType,propertyName);
		VariantManage->setValue(subitem,propertyData);
		groupItem->addSubProperty(subitem);
		rememberTheProperty(propertyIdName,subitem);
		return subitem;
	}
	///
	/// \brief ��һ���������ټ���һ����
	/// \param pG �����ָ��
	/// \param parentGroupItem ������
	/// \param groupName ����
	/// \param groupIdName ����id��־
	/// \return
	///
	QtProperty* addGroupInGroup(QtGroupPropertyManager * pG
		,QtProperty* parentGroupItem
		,const QString& groupName
		,const ID_TYPE& groupIdName)
	{
		QtProperty* groupItem = pG->addProperty(groupName);
		parentGroupItem->addSubProperty(groupItem);
		rememberTheProperty(groupIdName,groupItem);
		return groupItem;
	}
	QtProperty* addGroupInGroup(QtVariantPropertyManager * pVar
		,QtProperty* parentGroupItem
		,const QString& groupName
		,const ID_TYPE& groupIdName)
	{
		QtProperty* groupItem = pVar->addProperty(QtVariantPropertyManager::groupTypeId()
			,groupName);
		parentGroupItem->addSubProperty(groupItem);
		rememberTheProperty(groupIdName,groupItem);
		return groupItem;
	}
	///
	/// \brief �������Ա����е�double������Ŀ��ֵ
	/// \param doubleManage
	/// \param propertyIdName ���Ա���Ŀ���ַ���id
	/// \param propertyData ֵ
	/// \return �ɹ��޸ķ���true
	///
	bool setDoublePropertyData(QtDoublePropertyManager* doubleManage
		,const ID_TYPE& propertyIdName
		,double propertyData)
	{
		QtProperty* prop = m_id2Property[propertyIdName];
		if(nullptr == prop){
			return false;
		}
		doubleManage->setValue(prop,propertyData);
		return true;
	}
	//    bool setVarPropertyData(QtVariantPropertyManager* varManage
	//                                        , const QString& propertyIdName
	//                                        , QVariant varData)
	//    {
	//        QtProperty* prop = m_id2Property[propertyIdName];
	//        if(nullptr == prop){
	//            return false;
	//        }
	//        varManage->setValue(prop,varData);
	//        return true;
	//    }
	bool setVarPropertyData(const ID_TYPE& propertyIdName, QVariant varData)
	{
		QtVariantProperty* prop = static_cast<QtVariantProperty*>(m_id2Property[propertyIdName]);
		if(nullptr == prop){
			return false;
		}
		prop->setValue(varData);
		return true;
	}

	//     bool addGroupInTreeProperty(QtTreePropertyBrowser* treePro
	//                                 ,QtVariantPropertyManager * pVar
	//                                 ,const QString& groupName
	//                                 ,const QString& groupIdName)
	//     {
	//         pVar->addProperty(QtVariantPropertyManager::groupTypeId(),groupName);
	//     }

private:
	QMap<ID_TYPE,QtProperty*> m_id2Property;///< id��Ӧ������ָ��
	QMap<QtProperty*,ID_TYPE> m_Property2id;///<����ָ���Ӧ��id
};

#endif // QTPROPERTYIDSTORAGE_H
