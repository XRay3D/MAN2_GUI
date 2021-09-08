#pragma once

#include <QSettings>
#include <type_traits>

class DoubleSpinBox;
class QAbstractButton;
class QComboBox;
class QDoubleSpinBox;
class QGroupBox;
class QLineEdit;
class QPlainTextEdit;
class QRadioButton;
class QSpinBox;
class QTabWidget;

#define varName(val) val, #val

class MySettings : public QSettings {
    template <class T>
    struct fake_dependency {
        static bool constexpr value = false;
    };
    template <class T>
    using fake_dependency_v = typename fake_dependency<T>::value;

public:
    template <typename T>
    auto setValue(const QString& key, const T& value)
    {
        QSettings::setValue(key, value);
        return value;
    }

    template <typename T>
    auto getValue(const QString& key, T& value, const QVariant& defaultValue = QVariant()) const
    {
        value = QSettings::value(key, defaultValue).value<T>();
        return value;
    }

    template <typename W, typename = std::enable_if_t<std::is_member_function_pointer_v<decltype(&W::saveState)>>>
    void setValueGs(W* widget)
    {
        const QString name { widget->objectName() };
        assert(!name.isEmpty());
        QSettings::setValue(name + "_state", widget->saveState());
        QSettings::setValue(name + "_geometry", widget->saveGeometry());
    }

    template <typename W, typename = std::enable_if_t<std::is_member_function_pointer_v<decltype(&W::restoreState)>>>
    void getValueGs(W* widget) const
    {
        const QString name { widget->objectName() };
        assert(!name.isEmpty());
        widget->restoreState(QSettings::value(name + "_state").toByteArray());
        widget->restoreGeometry(QSettings::value(name + "_geometry").toByteArray());
    }

    template <typename W, typename = std::enable_if_t<std::is_base_of_v<QWidget, W>>>
    auto setValue(W* widget)
    {
        const QString name { widget->objectName() };
        assert(!name.isEmpty());

        if constexpr (std::is_base_of_v<QAbstractButton, W>) {
            QSettings::setValue(name, widget->isChecked());
            return widget->isChecked();
        } else if constexpr (std::is_base_of_v<QGroupBox, W>) {
            QSettings::setValue(name, widget->isChecked());
            return widget->isChecked();
        } else if constexpr (std::is_base_of_v<QDoubleSpinBox, W>) {
            QSettings::setValue(name, widget->value());
            return widget->value();
        } else if constexpr (std::is_same_v<W, QSpinBox>) {
            QSettings::setValue(name, widget->value());
            return widget->value();
        } else if constexpr (std::is_same_v<W, QComboBox>) {
            QSettings::setValue(name, widget->currentIndex());
            return widget->currentIndex();
        } else if constexpr (std::is_same_v<W, QLineEdit>) {
            QSettings::setValue(name, widget->text());
            return widget->text();
        } else if constexpr (std::is_same_v<W, QPlainTextEdit>) {
            QSettings::setValue(name, widget->toPlainText());
            return widget->toPlainText();
        } else if constexpr (std::is_same_v<W, QTabWidget>) {
            QSettings::setValue(name, widget->currentIndex());
            return widget->currentIndex();
        } else {
            fake_dependency_v<W>();
        }
    }

    template <typename W, typename D = QVariant, typename = std::enable_if_t<std::is_base_of_v<QWidget, W>>>
    auto getValue(W* widget, const D& defaultValue = {}) const
    {
        const QString name { widget->objectName() };
        assert(!name.isEmpty());

        if constexpr (std::is_base_of_v<QAbstractButton, W>) {
            widget->setChecked(QSettings::value(name, defaultValue).toBool());
            return widget->isChecked();
        } else if constexpr (std::is_base_of_v<QGroupBox, W>) {
            widget->setChecked(QSettings::value(name, defaultValue).toBool());
            return widget->isChecked();
        } else if constexpr (std::is_base_of_v<QDoubleSpinBox, W>) {
            widget->setValue(QSettings::value(name, defaultValue).toDouble());
            return widget->value();
        } else if constexpr (std::is_same_v<W, QSpinBox>) {
            widget->setValue(QSettings::value(name, defaultValue).toInt());
            return widget->value();
        } else if constexpr (std::is_same_v<W, QComboBox>) {
            widget->setCurrentIndex(QSettings::value(name, defaultValue).toInt());
            return widget->currentIndex();
        } else if constexpr (std::is_same_v<W, QLineEdit>) {
            widget->setText(QSettings::value(name, defaultValue).toString());
            return widget->text();
        } else if constexpr (std::is_same_v<W, QPlainTextEdit>) {
            widget->setPlainText(QSettings::value(name, defaultValue).toString());
            return widget->toPlainText();
        } else if constexpr (std::is_same_v<W, QTabWidget>) {
            widget->setCurrentIndex(QSettings::value(name, defaultValue).toInt());
            return widget->currentIndex();
        } else {
            fake_dependency_v<W>();
        }
    }

    template <typename V, typename = std::enable_if_t<std::is_fundamental_v<V>>>
    auto getValue(V& val, const char* name, V def) const
    {
        if constexpr (std::is_floating_point_v<V>) {
            val = QSettings::value(name, def).toDouble();
            return val;
        } else if constexpr (std::is_integral_v<V>) {
            val = QSettings::value(name, def).toInt();
            return val;
        } else {
            throw std::logic_error(typeid(V).name());
        }
    }

    template <typename V, typename = std::enable_if_t<std::is_fundamental_v<V>>>
    auto setValue(V val, const char* name)
    {
        QSettings::setValue(name, val);
        return val;
    }
};
