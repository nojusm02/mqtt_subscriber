include $(TOPDIR)/rules.mk

PKG_NAME:=mqtt_subscriber
PKG_RELEASE:=1
PKG_VERSION:=1.0.0

include $(INCLUDE_DIR)/package.mk

define Package/mqtt_subscriber
	CATEGORY:=Base system
	TITLE:=mqtt_subscriber
	DEPENDS:= +libmosquitto +libsqlite3 +libuci +libjson-c +libcurl
endef

define Package/mqtt_subscriber/description
	MQTT subscriber
endef

define Package/mqtt_subscriber/install
	$(INSTALL_DIR) $(1)/usr/bin $(1)/etc/init.d $(1)/etc/config
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mqtt_subscriber $(1)/usr/bin/
	#$(INSTALL_BIN) ./files/mqtt_subscriber.init $(1)/etc/init.d/mqtt_subscriber
	#$(INSTALL_CONF) ./files/mqtt_subscriber.config $(1)/etc/config/mqtt_subscriber
endef

$(eval $(call BuildPackage,mqtt_subscriber))

