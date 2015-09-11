next_version = 7.x-1.1-dev

#
#Seed versions of current release
#
#
datestamp=$(shell date +%s)
release_date=$(shell date -R)

set-version:
	$(call inject-version-info,.,obiba_mica)
	$(call inject-version-info,obiba_mica_commons,obiba_mica_commons)
	$(call inject-version-info,obiba_mica_dataset,obiba_mica_dataset)
	$(call inject-version-info,obiba_mica_model,obiba_mica_model)
	$(call inject-version-info,obiba_mica_network,obiba_mica_network)
	$(call inject-version-info,obiba_mica_search,obiba_mica_search)
	$(call inject-version-info,obiba_mica_study,obiba_mica_study)
	$(call inject-version-info,obiba_mica_app_angular,obiba_mica_app_angular)
	$(call inject-version-info,obiba_mica_data_access_request,obiba_mica_data_access_request)

inject-version-info = cd $1 && \
	sed -i "/^version/d" $2.info && \
	echo "version = $(next_version)" >> $2.info

