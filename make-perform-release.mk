#
#Seed versions of current release
#
#
include make-seed-versions.mk

#
#
#Push to GitHub
#
#
git-mica-release: github-release

#
# Push to Drupal.org
#

github-release:
	git checkout mica-drupal7-client-$(mica_branch_version) && \
	make inject-version-info && \
	echo "Enter a message for this tag push release?" && \
	read git_push_msg && \
	git tag -a $(mica_current_tag) -m "$$git_push_msg" && \
	git push upstream $(mica_current_tag)

