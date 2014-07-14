mica_version=1.0-dev
mica_branch=7.x-1.x
drupal_org_mica=git.drupal.org:project/obiba_mica.git

drupal_version = 7.27

#
# Mysql db access
#
db_name = drupal
db_user = root
db_pass = 1234


help:
	@echo
	@echo "Mica Drupal 7 Client"
	@echo
	@echo "Available make targets:"
	@echo "  all          : Clean & setup Drupal with a symlink to Mica modules in target directory and import drupal.sql"
	@echo "  setup-drupal : Setup Drupal with Mica modules in target directory"
	@echo

all: clean setup-drupal wwww import-sql settings enable-mica enable-obiba-auth devel bootstrap jquery_update cc

clean:
	rm -rf target

setup-drupal:
	drush make --prepare-install drupal/dev/drupal-basic.make target/drupal && \
	chmod -R a+w target/drupal && \
	ln -s $(CURDIR)/drupal/modules/mica_client $(CURDIR)/target/drupal/sites/all/modules/mica_client && \
	ln -s $(CURDIR)/drupal/modules/mica_bootstrap_config $(CURDIR)/target/drupal/sites/all/modules/mica_bootstrap_config && \
	ln -s $(CURDIR)/drupal/themes/mica_bootstrap $(CURDIR)/target/drupal/sites/all/themes/mica_bootstrap && \
	git clone https://github.com/obiba/drupal7-auth.git  $(CURDIR)/target/drupal/sites/all/modules/obiba_auth && \
	git clone https://github.com/obiba/drupal7-protobuf.git  $(CURDIR)/target/drupal/sites/all/modules/obiba_protobuf

wwww:
	sudo rm -f /var/www/html/drupal && \
	sudo ln -s $(CURDIR)/target/drupal /var/www/html/drupal && \
	sudo chown -R www-data:www-data /var/www/html/drupal

dump-sql:
	mysqldump -u $(db_user) --password=$(db_pass) --hex-blob $(db_name) --result-file="drupal/dev/drupal-$(drupal_version).sql"

import-sql:
	mysql -u $(db_user) --password=$(db_pass) -e "drop database if exists $(db_name); create database $(db_name);"
	mysql -u $(db_user) --password=$(db_pass) $(db_name) < "drupal/dev/drupal-$(drupal_version).sql"

settings:
	sed  's/@db_pass@/$(db_pass)/g' drupal/dev/settings.php > target/drupal/sites/default/settings.php
	cp drupal/dev/.htaccess target/drupal

enable-mica:
	cd target/drupal && \
	drush en -y mica_client

enable-obiba-auth:
	cd target/drupal && \
	drush en -y obiba_auth

bootstrap:
	cd target/drupal && \
	drush dl -y bootstrap && \
	drush en -y bootstrap && \
	drush en -y mica_bootstrap && \
	drush vset -y theme_default mica_bootstrap && \
	drush vset -y admin_theme seven

jquery_update:
	cd target/drupal && \
	drush dl -y jquery_update && \
	drush en -y jquery_update

devel:
	cd target/drupal && \
	drush dl -y devel && \
	drush en -y devel

cas:
	cd target/drupal && \
	drush dl -y cas && \
	drush en -y cas

cc:
	cd target/drupal && drush cc all


#
# Push to Drupal.org
#
git-push-mica:
	$(call clear-version-info,drupal/modules,mica_client) && \
	$(call clear-version-info,drupal/modules/mica_client,mica_client_study) && \
	$(call clear-version-info,drupal/modules/mica_client,mica_client_commons) && \
	$(call clear-version-info,drupal/modules/mica_client,mica_client_network) && \
	$(call clear-version-info,drupal/modules/mica_client,mica_client_model) && \
	$(call git-prepare,$(drupal_org_mica),obiba_mica,$(mica_branch)) . && \
	cp -r drupal/modules/mica_client/* target/drupal.org/obiba_mica && \
	$(call git-finish,obiba_mica,$(mica_branch))

clear-version-info = sed -i "/^version/d" $(1)/$2/$2.info && \
	sed -i "/^project/d" $(1)/$2/$2.info && \
	sed -i "/^datestamp/d" $(1)/$2/$2.info && \
	sed -i "/Information added by obiba.org packaging script/d" $(1)/$2/$2.info

git-prepare = rm -rf target/drupal.org/$(2) && \
	mkdir -p target/drupal.org && \
	echo "Enter Drupal username?" && \
	read git_username && \
	git clone --recursive --branch $(branch) $$git_username@git.drupal.org:project/$(2) target/drupal.org/$(2) && \
	cd target/drupal.org/$(2) && \
	git rm -rf * && \
	cd ../../..

git-finish = cd target/drupal.org/$(1) && \
	git add . && \
	git status && \
	echo "Enter a message for this commit?" && \
	read git_commit_msg && \
	git commit -m "$$git_commit_msg" && \
	git log